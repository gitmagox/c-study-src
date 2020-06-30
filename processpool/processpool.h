#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "map.h"


static const int MAX_PROCESS_NUMBER = 16;
static const int USER_PER_PROCESS = 65536;
static const int MAX_EVENT_NUMBER=10000;
#define PROCESS_NUM 4


static int sig_pipefd[2];

// //默认链接对象
// typedef struct client_data
// {

// 	char m_buf,//数据缓冲区
// 	int m_sockfd,
// 	sockaddr_in m_address,
// 	int m_read_idx;
// 	int m_epollfd,
// 	void (*init)(client_data * client_data,int epollfd,int sockfd,const sockaddr_in& client_addr),//初始化
// 	void (*process)(client_data * client_data),//逻辑处理
// } client_data;

//进程结构体
typedef struct mgx_process
{
	pid_t m_pid;
	int m_pipefd[2];
} mgx_process;

typedef struct processpool
{
	//保存所有子进程的描述信息
	mgx_process *  m_sub_process[PROCESS_NUM];
	//进程池中进程的总数量
	int m_process_number;
	//子进程的序号,m_idx=-1的时候为主进程
	int m_idx;
	//每一个进程都有一个内核事件表
	int m_epollfd;
	//监听socket
	int m_listenfd;
	//通过这个标误来确定进程是否停止运行
	_Bool m_stop;
	
} processpool;

//声明创建
processpool* mgx_process_create( int listendfd, int process_number);

void mgx_process_delete(processpool * mgx_processpool );

static int setnonblocking( int fd )
{
	int old_option = fcntl( fd, F_GETFL );
	int new_option  = old_option | O_NONBLOCK;
	fcntl( fd, F_SETFL, new_option );
	return old_option;
}

void addfd( int epollfd, int fd )
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
	setnonblocking( fd );
}

void removefd( int epollfd, int fd )
{
	epoll_ctl( epollfd, EPOLL_CTL_DEL, fd, 0 );
	close( fd );
}

void sig_handler( int sig )
{
	int save_errno = errno;
	int msg = sig;
	send( sig_pipefd[1], ( char* )&msg, 1, 0 );
	errno = save_errno;
}

void addsig( int sig, void(*handler)(int) ,_Bool restart)
{
	struct sigaction sa;
	memset( &sa, '\0', sizeof( sa ) );
	sa.sa_handler = handler;
	if( restart )
	{
		sa.sa_flags|= SA_RESTART;
	}
	sigfillset( &sa.sa_mask );
	assert( sigaction( sig, &sa, NULL )!=-1 );
}

//统一事件源
static void setup_sig_pipe(processpool * mgx_processpool)
{	
	mgx_processpool->m_epollfd = epoll_create( 5 );
	assert( mgx_processpool->m_epollfd != -1 );

	int ret = socketpair( PF_UNIX, SOCK_STREAM, 0, sig_pipefd );
	assert( ret != -1 );

	setnonblocking( sig_pipefd[1] );
	addfd( mgx_processpool->m_epollfd, sig_pipefd[0] );
	addsig( SIGCHLD, sig_handler, true );
	addsig( SIGTERM, sig_handler, true);
	addsig( SIGINT, sig_handler, true);
	addsig( SIGPIPE, SIG_IGN, true);

}

//进程池运行
void mgx_process_run(processpool *mgx_processpool)
{
	if( mgx_processpool->m_idx != -1 )
	{
		run_child(mgx_processpool);
		return ;
	}
	run_parent(mgx_processpool);
}


//清理进程池
void mgx_process_delete(processpool * mgx_processpool ){
	free(mgx_processpool->m_sub_process);
	free(mgx_processpool);
}

//子进程
void run_child(processpool *mgx_processpool)
{
	//父进程的通信管道
	int pipefd = mgx_processpool->m_sub_process[mgx_processpool->m_idx]->m_pipefd[1];

	addfd( mgx_processpool->m_epollfd, pipefd );

	struct epoll_event events[MAX_EVENT_NUMBER];
    map_void_t users;
    map_init(&users);

	int number = 0;
	int ret = -1;

	while( !mgx_processpool->m_stop )
	{
		number  = epoll_wait( mgx_processpool->m_epollfd, events, MAX_EVENT_NUMBER, -1 );
		if( ( number <0 ) && ( errno !=EINTR ) )
		{
			printf("epoll failure\n");
			break;
		}
		for( int i = 0; i < number; i++ )
		{
			int sockfd = events[i].data.fd;
			if( (sockfd == pipefd) && ( events[i].events & EPOLLIN ) )
			{
				int client = 0;
				ret = recv( sockfd, ( char* )&client, sizeof( client ), 0 );
				if( ((ret<0) && (errno != EAGAIN) ) || ret == 0 )
				{
					continue;
				}
				else
				{
					struct sockaddr_in client_address;
					socklen_t client_addrlength = sizeof( client_address );
					int connfd  = accept( mgx_processpool->m_listenfd, ( struct sockaddr* )&client_address,&client_addrlength );

					if( connfd <0 )
					{
						printf("errno is : %d\n",errno );
						continue;
					}
					addfd( mgx_processpool->m_epollfd, connfd );
                    users[connfd] = users[0]+connfd;
					users[connfd]->init( &users[connfd],mgx_processpool->m_epollfd,connfd,&client_address );
				}
			}else if( ( sockfd == sig_pipefd[0] ) && ( events[i].events & EPOLLIN ) )
			{
				int sig;
				char signals[1024];
				ret = recv( sig_pipefd[0], signals, sizeof( signals ), 0 );
				if( ret <= 0 )
				{
					continue;
				}
				else
				{
					for( int i = 0; i < ret; ++i )
					{
						switch( signals[i] )
						{
							case SIGCHLD:
							{
								pid_t pid;
								int stat;
								while( (pid = waitpid( -1, &stat, WNOHANG )) )
								{
									continue;
								}
								break;
							}
							case SIGTERM:
							case SIGINT:
							{
								mgx_processpool->m_stop = true;
								break;
							}
							default:
							{
								break;
							}
						}
					}
				}
			}else if(mgx_processpool->m_listenfd!=sockfd  && events[i].events & EPOLLIN )
			{

				users[sockfd]->process(&users[sockfd]);
			}
			else
			{
				continue;
			}
		}
	}
    for (int j = 0; j < USER_PER_PROCESS; ++j)
    {
        free(users[j]);
    }
    free( users );
	close( pipefd );
	close( mgx_processpool->m_epollfd );
}

//父进程
void run_parent(processpool  *mgx_processpool)
{


	addfd( mgx_processpool->m_epollfd,mgx_processpool->m_listenfd );

	struct epoll_event events[ MAX_EVENT_NUMBER ];

	int sub_process_counter = 0;
	int new_conn = 1;
	int number = -1;
	int ret = -1;

	while( ! mgx_processpool->m_stop )
	{

		number = epoll_wait( mgx_processpool->m_epollfd, events, MAX_EVENT_NUMBER, -1 );

		if( (number<0) && (errno!=EINTR) )
		{
			printf("epoll failure\n");
			break;
		}

		for( int i=0; i< number; i++ )
		{
			int sockfd  = events[i].data.fd;
			if( sockfd == mgx_processpool->m_listenfd )
			{
				int i = sub_process_counter;
				do
				{
					if( mgx_processpool->m_sub_process[i]->m_pid != -1 )
					{
						break;
					}
					i = (i+1)%mgx_processpool->m_process_number;
				}while( i != sub_process_counter );

				if( mgx_processpool->m_sub_process[i]->m_pid == -1 )
				{
					mgx_processpool->m_stop = true;
					break;
				}

				sub_process_counter = (i+1)%mgx_processpool->m_process_number;
				send( mgx_processpool->m_sub_process[sub_process_counter]->m_pipefd[0], (char*)&new_conn, sizeof(new_conn), 0 );
				printf("send request to child %d\n",sub_process_counter );
			}
			else if( (sockfd == sig_pipefd[0]) && ( events[i].events & EPOLLIN ) )
			{
				int sig;
				char signals[1024];
				ret  = recv( sig_pipefd[0], signals, sizeof( signals ), 0 );
				if( ret <= 0 )
				{
					continue;
				}
				else
				{
					for( int i = 0; i < ret; ++i )
					{
						switch( signals[i] )
						{
							case SIGCHLD:
							{
								pid_t pid;
								int stat;
								while( (pid=waitpid( -1, &stat, WNOHANG )) >0 )
								{
									for( int i = 0; i < mgx_processpool->m_process_number; ++i )
									{
										if( mgx_processpool->m_sub_process[i]->m_pid == pid )
										{
											printf( "child %d join\n", i );
											close( mgx_processpool->m_sub_process[i]->m_pipefd[0] );
											mgx_processpool->m_sub_process[i]->m_pid = -1;
										}
									}
								}
								mgx_processpool->m_stop = true;
								for( int i = 0; i < mgx_processpool->m_process_number; ++i )
								{
									if( mgx_processpool->m_sub_process[i]->m_pid != -1 )
									{
										mgx_processpool->m_stop = false;
									}
								}
								break;
							}
							case SIGTERM:
							case SIGINT:
							{
								printf("kill all the clild now\n");
								for( int i = 0; i < mgx_processpool->m_process_number; ++i )
								{
									int pid = mgx_processpool->m_sub_process[i]->m_pid;
									if( pid != -1 )
									{
										kill( pid, SIGTERM );
									}
								}
								break;
							}
							default:
							{
								break;
							}
						}
					}
				}
			}
		}
	}
}

//创建进程池
processpool* mgx_process_create( int listendfd, int process_number)
{
	struct processpool * mgx_processpool;
	mgx_processpool = (processpool *)malloc( sizeof(processpool));
	mgx_processpool->m_listenfd = listendfd;
	mgx_processpool->m_process_number = PROCESS_NUM;
	mgx_processpool->m_idx = -1;
	mgx_processpool->m_stop = false;
	assert( ( PROCESS_NUM >0 ) && PROCESS_NUM <= MAX_PROCESS_NUMBER );
	//分配内存
	mgx_processpool->m_sub_process[0] = (mgx_process *)malloc( sizeof(mgx_process) * PROCESS_NUM );

    setup_sig_pipe(mgx_processpool);
	for( int i=0; i < PROCESS_NUM; ++i )
	{
        mgx_processpool->m_sub_process[i] = mgx_processpool->m_sub_process[0]+i;
		int ret = socketpair( PF_UNIX, SOCK_STREAM, 0, mgx_processpool->m_sub_process[i]->m_pipefd );
		assert( ret == 0 );

		int pid = fork();

		mgx_processpool->m_sub_process[i]->m_pid = pid;
		assert(pid>=0 );
		if( pid > 0 )
		{
			close( mgx_processpool->m_sub_process[i]->m_pipefd[1] );
			continue;
		}
		else
		{
			close( mgx_processpool->m_sub_process[i]->m_pipefd[0] );
			mgx_processpool->m_idx = i;
			break;
		}
	}

	return mgx_processpool;

}

#undef _client_data_type
#endif


