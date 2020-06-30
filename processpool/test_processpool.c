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

static const int BUFFER_SIZE = 1024;

//默认链接对象
typedef struct client_data
{
	char *m_buf;//数据缓冲区
	int m_sockfd;
	struct sockaddr_in m_address;
	int m_read_idx;
	int m_epollfd;
	void (*init)(struct client_data * client_data,int epollfd,int sockfd,struct sockaddr_in *client_addr);//初始化
	void (*process)(struct client_data * client_data);//逻辑处理
} client_data;

//初始化
void init(client_data * client_data,int epollfd,int sockfd,struct sockaddr_in *client_addr)
{
	client_data->m_epollfd = epollfd;
	client_data->m_sockfd = sockfd;
	client_data->m_address = *client_addr;
	memset(client_data->m_buf,'\0',BUFFER_SIZE);
	client_data->m_read_idx = 0;
}

void process(client_data *client_data)
{
	int idx = 0;
	int ret = -1;
	while( true )
	{
		idx = client_data->m_read_idx;
		ret = recv( client_data->m_sockfd, client_data->m_buf+idx,BUFFER_SIZE-1-idx, 0 );
		if( ret < 0 )
		{
			if( errno != EAGAIN )
			{
				removefd( client_data->m_epollfd , client_data->m_sockfd);
			}
			break;
		}
		else if( ret == 0 )
		{
			client_data->m_read_idx += ret;
			printf("user content is:%s\n",client_data->m_buf );

			for( ; idx <client_data->m_read_idx; ++idx )
			{
				if( (idx >=1) && (client_data->m_buf[idx-1] == '\r') && (client_data->m_buf[idx]=='\n') )
				{
					break;
				}
			}
			if(idx == client_data->m_read_idx)
			{
				continue;
			}
			client_data->m_buf[idx-1] = '\0';

			char* file_name = client_data->m_buf;

			if( access( file_name, F_OK ) == -1 )
			{
				removefd( client_data->m_epollfd, client_data->m_sockfd );
				break;
			}
			else if( ret >0 )
			{
				removefd( client_data->m_epollfd, client_data->m_sockfd );
				break;
			}
			else
			{
				close( STDOUT_FILENO );
				dup( client_data->m_sockfd );
				execl( client_data->m_buf, client_data->m_buf,0 );
				exit( 0 );
			}
		}
	}
}

#define _client_data_type client_data

#include "processpool.h"//进程池

int main( int argc, char * argv[] )
{
	if( argc <=2 )
	{
		printf("usage:%s ip_address port_number\n",basename(argv[0]) );
		return 1;
	}

	const char* ip = argv[1];
	int port = atoi( argv[2] );

	int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
	assert( listenfd >= 0 );

	int ret  = 0;
	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );

	ret  = bind( listenfd, (struct sockaddr*)&address,sizeof( address ) );
	assert( ret != -1 );
	ret = listen( listenfd, 5 );

	assert(ret !=-1);

	struct processpool *my_processpool;

	my_processpool = mgx_process_create(listenfd, 4);

	mgx_process_run(my_processpool);

	mgx_process_delete(my_processpool);
	
	close(listenfd);

	return 0;

}


