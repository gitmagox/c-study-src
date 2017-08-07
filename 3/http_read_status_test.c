#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096 /* 读缓冲区大小 */
/* 定义主状态机 的两种状态 分别表示:当前正在分析的请求行，当前正在分析头部字段*/
enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER }
/* 从状态机的三种可能状态,即行的读取状态，分别表示 :读取到一个完整的行，行出错和行数据尚且不完整 */
enum LINE_STATE { LINE_OK = 0, LINE_BAD, LINE_OPEN };
/* 服务器分析处理http请求的结果:
 *NO_REQUEST 表示请求不完整，需要继续读取客户数据;
 *GET_REQUEST 表示取得了一个完整的客户请求;
 *BAD_REQUEST 表示客户请求有语法错误;
 *FORBIDDEN_REQUEST 表示客户对资源没有足够的访问权限;
 *INTERNAL_ERROR 表示服务器内部错误; 
 *CLOSED_CONNECTION 表示客户端已经关闭连接了;
 * */
enum HTTP_CODE {
	NO_REQUEST,
	GET_REQUEST,
	BAD_REQUEST,
	FORBIDDEN_REQUEST,
	INTERNAL_ERROR,
	CLOSED_CONNECTION
};
/*  此处简化应答 */
static const char* szret[] = { "I get a correct request\n", "something wrong\n"  };

/* 从状态机 */
LINE_STATUS parse_line( char* buffer, int& checked_index, int& read_index ){

	for( ;checked_index < read_index; ++checkked_index )
	{	
		//当前字符
		temp = buffer[ checked_index ];
		if( temp == "\r" )
		{
			if ( ( checked_index +1 ) == read_index )
			{
				return LINE_OPEN;
			}
			else if ( buffer[ checked_index +1 ] == '\n' )
			{
				buffer[ checked_index++ ] = '\0';
				buffer[ checked_index++ ] = '\0';
			       	return LINE_OK
			}
		}
		else if( temp == "\n" )
		{
			if( buffer[ checked-1 ] == '\r' && checked_index > 1  )
			{
				buffer[ checked_index ] = '\0';
				buffer[ checked_index++ ] = '\0';
				return LINE_OK;
			}
		}
	}
	return LINE_OPEN;
}
