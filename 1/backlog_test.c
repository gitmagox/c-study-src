#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//监听开关
static _Bool stop = false;

/* SIGTERM信号处理函数*/
static void handle_term( int sig )
{
	stop = true;
}

int main(int argc,char* argv[]){
	
	signal( SIGTERM, handle_term );

	if( argc<=3 )
	{
		printf(" usage: %s ip_address port_number backlog\n ");
		return 1;
	}

	//取第一个参数ip地址
	const char* ip = argv[1];
	//取第二个参数端口
	int port = atoi( argv[2] );
	//取第三个参数
	int backlog = atoi( argv[3] );
	
	int sock = socket( PF_INET, SOCK_STREAM,0 );
	assert( sock >=0 );

	/*创建一个ipv4 socket 地址
	 *struct sockaddr_in
	 {
	 	sa_family_t sin_family;   //地址族:AF_INET
		u_intl6_t sin_port;       //端口号
		struct in_addr sin_addr;  //IPv4地址结构体
	 };
	 struct in_addr
	 {
	 	u_int32_t s_addr; //用网络字节表示ipv4地址
	 }
	 * */
	
	struct sockaddr_in address;
	//bzero函数只能用于linux下,可以用memset()函数代替)
	//此函数的作用是用于对指针指定的前多大的清零
	bzero( &address, sizeof(address) );
	//设置地址，协议
	/*
	 *       协议         地址         描述
	 *	PF_UNIX	     AF_UNIX    UNIX本地域协议
	 *	PF_INET      AF_INET    TCP/IPv4 协议
	 *	PF_INIT6     AF_INET6   TCP/IPv6 协议	
	 */
	address.sin_family = AF_INET;
	//ip地址转换
	inet_pton( AF_INET, ip, &address.sin_addr  );
	//端口设置
	address.sin_port = htons( port );
	

	int ret = bind( sock,( struct sockaddr* )&address,sizeof( address ) );
	assert( ret !=1 );

	/*等待连接，直到sigterm信号中断*/

	while ( !stop )
	{
		sleep(1);
	}
	
	//关闭socket
	close( sock );
	return 0;
}
