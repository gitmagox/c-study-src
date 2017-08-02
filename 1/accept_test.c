#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main ( int argc, char* argv[] )
{
	if( argc <=2 )
	{
		printf("usage:%s ip_address port_number\n",basename( argv[0] ));
		return 1;
	}
	//接收ip
	const char* ip = argv[1];
	int port = atoi( argv[2] );

	//定义ipv4协议
	struct sockaddr_in address;
	//清零结构体内存范围
	bzero( &address, sizeof(address) );
	//设置协议
	address.sin_family = AF_INET;
	//设置ip
	inet_pton( AF_INET, ip,  &address.sin_addr );
	//转换port为网络字节,并设置
	address.sin_port = htons( port );
	
	//创建sock支持tcp字符流
	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	//断言处理
	assert( sock >=0 );

	//设置sock命名
	int ret = bind( sock, (struct sockaddr*)&address,sizeof(address) );
	assert( ret !=-1 );
	//设置监听
	ret = listen( sock,5 );
	assert( ret !=-1 );
	/* 这里暂停20秒以等待客户端连接和相关操作（掉线或退出）完成
	 *
	 * 经过测试得出结论，accept只是从监听队列中取出连接，不论连接处于何种状态
	 * （如ESTABLISHED CLOSE_WAIT），不会关心任何网络状况的变化
	 *
	 * */
	sleep(20);
	
	//创建一个客户端的协议结构
	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof( client );
	//从监听列表接收一个客户端连接
	int connfd = accept( sock,( struct sockaddr*)&client,&client_addrlength);
	if( connfd < 0 )
	{
		printf("errno is: %d\n",errno);
	}
	else
	{
		/*接受成功则打印出客户端的ip地址和端口号*/
		char remote[INET_ADDRSTRLEN];
		printf("connected with ip: %s and port: %d\n",
				inet_ntop( AF_INET,&client.sin_addr, remote,INET_ADDRSTRLEN ),
				ntohs( client.sin_port ) );
		close( connfd );
	}

	close( sock );
	return 0;
}
