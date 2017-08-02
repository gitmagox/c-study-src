#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 1024

int main( int argc, char*argv[] )
{
	if( argc <=2 )
	{
		printf("usage:%s ip_address port_numger\n",basename( argv[0] ));
		return 1;
	}
	//接收参数
	const char* ip = argv[1];
	int port = atoi( argv[2] );

	//定议地址
	struct sockaddr_in address;
	bzero( &address, sizeof(address) );
	//设置ip和端口转换相应的网络字节
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );
	//创建socket(tcp流)
	int sock = socket( PF_INET, SOCK_STREAM,0 );
	//设置断言
	assert( sock >= 0 );
	//邦定命名
	int ret = bind( sock, (struct sockaddr*)&address, sizeof(address));
	//设置断言
	assert( ret != -1 );
	//设置监听
	ret = listen( sock,5 );
	//设置断言
	assert( ret != -1 );

	//定议客户地址
	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof( client );
	//接受客户连接
	int connfd = accept( sock, ( struct sockaddr* ) &client, &client_addrlength );
	if ( connfd < 0  )
	{
		printf("errno is: %d\n", errno );
	}
	else
	{
		//设置接收缓冲区
		char buffer[ BUF_SIZE ];
		
		//清零缓冲区
		memset( buffer, '\0', BUF_SIZE );
		ret = recv( connfd, buffer, BUF_SIZE-1, 0 );
		printf( "got %d bytes of normal data '%s'\n", ret, buffer );
		
		/*
		 *经过测试,OOB 外带数据，只接收到最后一个字节 
		 */
		memset( buffer, '\0', BUF_SIZE );
		ret = recv( connfd, buffer, BUF_SIZE, MSG_OOB );
		printf( "got %d bytes of oob data '%s'\n", ret, buffer );

		//接收正常数据
		memset( buffer, '\0', BUF_SIZE );
		ret = recv( connfd, buffer, BUF_SIZE, 0 );
		printf( "got %d bytes of normal data '%s'\n", ret, buffer );

		close( connfd );
	}
	
	//关闭
	close( sock );
	return 0;
}
