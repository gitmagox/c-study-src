#define _GNU_SOURCE 1
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
#include <poll.h>

#define USER_LIMIT 5 /* 最大用户数量 */
#define BUFFER_SIZE 64 /* 读缓冲区的大小 */
#define FD_LIMIT 65535 /* 文件描述符数量限制 */
/* 客户数据; 客户端socket 地址,待写到客户端的数据的位置,从客户端读入的数据 */
struct client_data
{
	sockaddr_in address;
	char* write_buf;
	char buf[ BUFFER_SIZE ];
};
typedef struct client_data client_data;

int setnonblocking( int fd )
{
	int old_option = fcntl( fd, F_GETFL );
	int new_option = old_option | O_NONBLOCK;
	fcntl( fd, F_SETFL, new_option );
	return old_option;
}

int main( int argc, char* argv[] )
{
	if( argc <= 2 )
	{
		printf("usage: %s ip_address port_number\n", basename( argv[0] ) );
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi( argv[2] );

	int ret = 0
	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );

	int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
	assert( listenfd > 0 );

	ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
	assert( ret != -1 );

	ret = listen( listenfd, 5 );
	assert( ret != -1 );

	/* 创建数组分配FD_LIMITW */
	//c++中的写法;
	client_data* users = ( client_data * )malloc( ( FD_LIMIT )*sizeof( client_data ) );

	return 1;
}