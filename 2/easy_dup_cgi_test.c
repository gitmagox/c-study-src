#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main( int argc, char* argv[] )
{
	if( argc <=2 )
	{	
		printf("usage: %s ip_address port_number\n",basename( argv[0] ));
		return 1;
	}
	const char*ip = argv[1];
	int port = atoi( argv[2] );
	//ipv4地址
	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );
	//创建sock
	int sock = socket( PF_INET, SOCK_STREAM ,0 );
	assert( sock >=0 );
	//邦定命名
	int ret = bind( sock, ( struct sockaddr* )&address,sizeof( address ) );
	assert( ret != -1 );
	//监听
	ret = listen( sock, 5 );
	assert( ret != -1 );
	//客户端地址ipv4,接爱连接
	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof( client );
	int connfd = accept( sock, ( struct sockaddr* )&client, &client_addrlength );
	if ( connfd < 0 )
	{
		printf( "errno is: %d\n", errno );
	}
	else
	{
		close( STDOUT_FILENO );
		//printf打印到终端的内容直接输出到客户连接的对应的socket上
		dup( connfd );
		printf( "abcdef\n" );
		close( connfd );
	}

	close( sock );
	return 0;
}
