#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define __USE_GNU
#include <fcntl.h>


int main( int argc, char* argv[] )
{
	if( argc <= 2 )
	{
		printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
		return 1;
	}

	//接爱参数
	const char* ip = argv[1];
	int port = atoi( argv[2] );

	//定议地址
	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	//设置ip ipv4协议
	address.sin_family  = AF_INET;
	inet_pton( AF_INET, ip ,&address.sin_addr );
	//设置端口
	address.sin_port = htons( port );

	//创建sock
	int sock  = socket( PF_INET, SOCK_STREAM , 0 );
	assert( sock >= 0 );

	int ret = bind( sock, (struct sockaddr* )&address, sizeof( address ) );
	assert( ret >=0 );

	ret = listen( sock, 5 );

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof( client );
	int connfd = accept( sock, ( struct sockaddr* )&client, &client_addrlength );
	if ( connfd < 0 )
	{
		printf( "errno is: %d\n",errno );
	}
	else
	{       //回射
		int pipefd[2];
		assert( ret != -1 );
		ret = pipe( pipefd ); /*创建管道*/
		/* 将connfd上流入的客户数据定向到管道中 */
		/*
		 *#include <fcntl.h>
		 *ssize_t splice ( int fd_in, loff_t* off_in, int fd_out, loff_t* off_out, size_t len, unsigned int flags );
		 *fd_in:待输入数据的文件描述符.如果fd_in是一个管道，那么off_in必段设置为NUll。如果不是一个管道文件描述符（比如socket），
 		 *那么off_in 表示从输入数据流的何处开始读取数据.此时如果设置为null，则表示从输入数据流的当前偏移位置读入.如果不为空
		 *刚它将指出具体的偏移位置 fd_out off_out 相同		 
		 *flags参数 SPLICE_F_MOVE:按整页内存移动数据 SPLICE_F_NONBLOCK 非阻塞splice操作,还会受文件描述符本身的状态影响
		 *SPLICE_F_MORE:给内核的一个提示，后续的splice调用将读取更多数据
		 *SPlICE_F_GIFT 对splice没有效果
		 */
		ret = splice( connfd, NULL, pipefd[1], NULL ,32768, SPLICE_F_MORE | SPLICE_F_MOVE );
		assert( ret !=-1 );
		//将管道的输出定向到connfd客户连接文件描述符
		ret = splice( pipefd[0], NULL, connfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE );
		assert( ret !=-1 );
		close( connfd );				
	
	}
	
	close( sock );
	return 0;
}
