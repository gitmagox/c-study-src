#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

int main( int argc, char*argv[] )
{
	if( argc <= 2 )
	{
		printf("usage: %s ip_address port_number send_bufer_size\n",basename( argv[0] ));
		return 1;
	}
	const char* ipo = argv[1];
	int port  = atoi( argv[2] );
	//定义服务地址
	struct sockaddr_in server_address;
	//地址结构体清零
	bzero( &server_address, sizeof( server_address ) );
	//设置为ipv4协议
	server_address.sin_family = AF_INET;
	//创建socket 连接
	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	//设置断言
	assert( sock >=0 );
	
	//设置发送
	int sendbuf = atoi( argv[3] );
	int len = sizeof( sendbuf );
	//设置tcp 发送缓冲区大小,然后立即读取之
	setsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof( sendbuf ) );
	getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, ( socklen_t* )&len );

	printf( "the tcp send buffer size after setting is %d\n",sendbuf );

	//连接服务端
	if( connect( sock, ( struct sockaddr*)&server_address, sizeof( server_address ) ) )
	{
		char buffer[ BUFFER_SIZE ];
		//清零
		memset( buffer, 'a', BUFFER_SIZE );
		send( sock, buffer, BUFFER_SIZE, 0 );
	}
	close( sock );
	return 0;
}
