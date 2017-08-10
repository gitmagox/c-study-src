/*  EPOLL LT 和 ET 模式 */
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
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10
/* 将文件描述符设置成非阻塞 */
int setnonblocking( int fd )
{
	int old_option = fcntl( fd, F_GETFL );
	int new_option = old_option | O_NONBLOCK;
	fcntl( fd, F_SETFL, new_option );
	return old_option;
}
/* 将文件描述符 fd 上的EPOLLIN 注册到 epollfd 指示的epoll 内核事件表中，
参数enable_et 指定是否对fd启用ET模式  */
void addfd( int epollfd, int fd, _Bool enable_et )
{
	epool_event event;
	/*
		struct epool_event 
		{
			_uint32_t events; //epoll事件
			epoll_data_t data; //用户数据
		}

		typedef union epoll_data
		{
			void* ptr;
			int fd;
			unit32_t u32;
			unit64_t u64;
		} epoll_data_t;
		注意:epoll_data_t是一个联合体，所以成员请单独使用，如果需要将文件描述符和用户
		数据关联起来，最好把fd,和其他用户数据绑定在一起存放在ptr中，实现快速的数据访问;
	 */
	event.data.fd = fd;
	//数据可读事件
	event.events = EPOLLIN;
	//切换工作方式
	if ( enable_et )
	{
		event.events != EPOLLET;
	}
	//添加epoll事件
	epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
	//设置文件为非阻塞方式
	setnonblocking( fd );
}

/* LT模式的工作流程 */
void lt( epoll_event* events, int number, int epollfd, int listenfd )
{
	char buf[ BUFFER_SIZE ];

	for ( int i = 0; i < number; i++ )
	{
		int sockfd = events[i].data.fd;
		//如果是临听的fd
		if( sockfd == listenfd )
		{
			struct sockaddr_in client_address;
			socklen_t client_addrlength = sizeof( client_address );
			int connfd = accept( listenfd, ( struct sockaddr* )&client_address,
									&client_addrlength );
			addfd( epollfd, connfd, false ); /* 对connfd禁用ET模式 */
		}
		else if ( events[i].events & EPOLLIN )
		{
			/* 只要socket 读取缓存中还有没读出的数据，这段代码就被触发 */
			printf( "event trigger once\n" );
			memset( buf, '\0', BUFFER_SIZE );
			int ret = recv( sockfd, buf, BUFFER_SIZE-1, 0 );
			if( ret <= 0 )
			{
				close( sockfd );
				continue;
			}

			printf("event trigger once\n");
			memset( buf, '\0', BUFFER_SIZE );
			int ret = recv( sockfd, buf, BUFFER_SIZE, 0 );
			if ( ret <= 0 )
			{
				close( sockfd );
				continue;
			}
			printf( "get %d bytes of content: %s\n", ret, buf );
		}
		else
		{
			printf( "Something else happened\n" );
		}
	}
}