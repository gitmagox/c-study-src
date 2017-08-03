#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int main( int argc, char *argv[]  )
{
	//断言
	assert( argc==2 );
	//取主机名
	char *host = argv[1];

	/*
	 *能过主机名取主机的地址信息
	 struct hostent
	 {
	 	char* h_name;  //主机名
		char** h_aliases; //主机别名列表，可能有多个
		int h_addrtype; //地址类型
		int h_length; //地址长度
		char** h_addr_list  //二级指针 主机Ip地址列表
	 }
  	 示例:
  	#include <netdb.h>	 
	struct hostent* gethostbyname( const char*name );
      	struct hostent* gethostbyaddr( const void* addr, size_t len, int type )	
	 *
	 */
	struct hostent* hostinfo = gethostbyname( host );
	assert( hostinfo );
	/*
	 *取服务信息
	 struct server
	 {
	 	char* s_name; //服务名称
		char** s_aliases; //服务名称列表，可能会有多个
	       	int s_port:	//端口号
		char* s_proto; //服务类型	
	 }
	 #include <netdb.h>
	 struct server* getservbyname( const char* name, const char* proto );
	 struct server* getservbyport( int port,const char* proto );
	 */
	//取daytime 服务信息
	struct servent* servinfo = getservbyname( "daytime", "tcp" );
	assert( servinfo );
	printf( "daytime port is %d\n",ntohs( servinfo->s_port ) );

	//定义服务地址结构体
	struct sockaddr_in address;
	//设置为ipv4协议地址
	address.sin_family = AF_INET;
	//设置端口
	address.sin_port = servinfo->s_port;
	/*设置IP网络字节序，取hostinfo结构体中的char** h_addr_list一个指向in_addr类型的二级指针*/
	address.sin_addr = *( struct in_addr* )*hostinfo->h_addr_list;
	
	//创建socket tcp字节流
	int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	//与daytime服务建立连接
	int result = connect( sockfd, (struct sockaddr* )&address, sizeof( address ) );
	assert( result !=-1);
         
	char buffer[128];
	result = read( sockfd, buffer,sizeof( buffer ) );
	assert( result >0 );
	buffer[ result ] = '\0';
	printf( "the day time is: %s",buffer );
	close( sockfd );

	return 0;
}
