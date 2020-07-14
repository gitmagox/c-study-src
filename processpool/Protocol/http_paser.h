//
// Created by magox on 7/9/20.
//

#ifndef PROCESSPOOL_HTTP_PASER_H
#define PROCESSPOOL_HTTP_PASER_H

#include "protocol.h";
#include "request.h";


enum HTTP_METHOD {OPTIONS,GET,HEAD,POST,PUT,DELETE,TRACE,CONNECT};

static const char *http_method_strings[] ={
        "OPTIONS","GET", "HEAD" , "POST", "PUT" , "DELETE" , "TRACE", "CONNECT"
};

enum HTTP_STATUS_CODE {
    HTTP_CODE_100,HTTP_CODE_101,
    HTTP_CODE_200,HTTP_CODE_201,HTTP_CODE_202,HTTP_CODE_204,HTTP_CODE_206,
    HTTP_CODE_300,HTTP_CODE_301,HTTP_CODE_302,HTTP_CODE_303,HTTP_CODE_304,
    HTTP_CODE_305,HTTP_CODE_307,HTTP_CODE_308,
    HTTP_CODE_400,HTTP_CODE_401,HTTP_CODE_402,HTTP_CODE_403,HTTP_CODE_404,HTTP_CODE_405,
    HTTP_CODE_406,HTTP_CODE_407,HTTP_CODE_408,HTTP_CODE_409,HTTP_CODE_410,HTTP_CODE_411,
    HTTP_CODE_412,HTTP_CODE_413,HTTP_CODE_414,HTTP_CODE_415,HTTP_CODE_416,HTTP_CODE_417,
    HTTP_CODE_500,HTTP_CODE_501,HTTP_CODE_502,HTTP_CODE_503,HTTP_CODE_504,HTTP_CODE_505
};

static const char *http_code_status_string[]={
        "100 Section Continue",
        "101 Switching Protocols",
        "200 OK","201 Created","202 Accepted","203 Non-Authoritative Information",
        "204 No Content","205 Reset Content","206 Partial Content",
        "300 Multiple Choices","301 Moved Permanently","302 Found","303 See Other",
        "304 Not Modified","305 Use Proxy","307 Temporary Redirect","308 Temporary Redirect",
        "400 Bad Request","401 Unauthorized","402 1Payment Required","403 Forbidden","404 Not Found",
        "405 Method Not Allowed","406 Not Acceptable","407 Proxy Authentication Required","408 Request Time-out",
        "409 Conflict","410 Gone","411 Length Required","412 Precondition Failed","413 Request Entity Too Large",
        "414 Request-URI Too Large","415 Unsupported Media Type","416 1Requested range not satisfiable",
        "417 Expectation Failed",
        "500 Internal Server Error","501 Not Implemented","502 Bad Gateway",
        "503 ervice Unavailable","504 Gateway Time-out","505 HTTP Version not supported"};

enum STRTOK {CR,LF,SP,CRLF};
static const char *strtok_string[] ={"\r","\n", " " , "\r\n"};

#define http_strtok(T) strtok_string[T]
#define http_method_string(T) http_method_strings[T]
#define http_code_string(T) http_code_status_string[T]

#define  isHeaderKey(T)  strncasecmp( temp, T, sizeof(T)-1 )  == 0

/* 定义主状态机 的两种状态 分别表示:当前正在分析的请求行，当前正在分析头部字段*/
typedef enum {
    CHECK_STATE_REQUESTLINE = 0,
    CHECK_STATE_HEADER
}CHECK_STATE;
//typedef enum { CHECK_STATE_REQUESTLINE = 0; CHECK_STATE_HEADER }CHECK_STATE;
/* 从状态机的三种可能状态,即行的读取状态，分别表示 :读取到一个完整的行，行出错和行数据尚且不完整 */
typedef enum{ LINE_OK = 0,LINE_BAD,LINE_OPEN }LINE_STATUS;
/*
 *服务器分析处理http请求的结果:
 *NO_REQUEST 表示请求不完整，需要继续读取客户数据;
 *GET_REQUEST 表示取得了一个完整的客户请求;
 *BAD_REQUEST 表示客户请求有语法错误;
 *FORBIDDEN_REQUEST 表示客户对资源没有足够的访问权限;
 *INTERNAL_ERROR 表示服务器内部错误;
 *CLOSED_CONNECTION 表示客户端已经关闭连接了;
 */
typedef enum {
    NO_REQUEST,
    GET_REQUEST,
    BAD_REQUEST,
    FORBIDDEN_REQUEST,
    INTERNAL_ERROR,
    CLOSED_CONNECTION,
}HTTP_CODE;
/*  此处简化应答 */
static const char* szret[] = { "I get a correct request\n", "something wrong\n"  };

/* 从状态机 */
LINE_STATUS http_parse_line( char* buffer, int* checked_index, int* read_index,http_request* httpRequest ){

    char temp;
    for( ;*checked_index < *read_index; ++*checked_index )
    {
        //当前字符
        temp = buffer[ *checked_index ];
        if( temp == '\r' )
        {
            if ( ( *checked_index +1 ) == *read_index )
            {
                return LINE_OPEN;
            }
            else if ( buffer[ *checked_index +1 ] == '\n' )
            {
                buffer[ (*checked_index)++ ] = '\0';
                buffer[ (*checked_index)++ ] = '\0';
                return LINE_OK;
            }
        }
        else if( temp == '\n' )
        {
            if( buffer[ *checked_index-1 ] == '\r' && *checked_index > 1  )
            {
                buffer[ (*checked_index)-- ] = '\0';
                buffer[ (*checked_index)++ ] = '\0';
                return LINE_OK;
            }
        }
    }
    return LINE_OPEN;
}

/* 分析请求行 */
HTTP_CODE http_parse_requestline( char* temp, CHECK_STATE* checkstate,http_request* httpRequest )
{
    char* url = strpbrk( temp, " \t" );
    /* 如果请求行中没有空白字符或"\t"字符,则http请求必有问题 */
    if ( ! url )
    {
        return BAD_REQUEST;
    }
    *url++ = '\0';

    char* method = temp;
    if ( strcasecmp( method, "GET" ) == 0 ) /* 仅支持GET方法 */
    {
        printf( "The request method is GET\n" );
    }
    else
    {
        return BAD_REQUEST;
    }

    url += strspn( url, " \t" );
    char* version = strpbrk( url, " \t" );
    if ( ! version )
    {
        return BAD_REQUEST;
    }
    *version++ = '\0';
    version += strspn( version, " \t" );
    /* 仅支持 HTTP/1.1 */
    if ( strcasecmp( version, "HTTP/1.1" ) != 0 )
    {
        return BAD_REQUEST;
    }
    /* 检查 url 是否合法 */
    if ( strncasecmp( url, "http://", 7  ) == 0 )
    {
        url += 7;
        url = strchr( url, '/' );
    }
    if( ! url || url[ 0 ] != '/' )
    {
        return	BAD_REQUEST;
    }
    printf( "The request URL is : %s\n", url );
    /* HTTP 请求行处理完毕,状态转移到头部字段的分析 */
    *checkstate = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

/* 分析头部字段 */
HTTP_CODE http_parse_headers( char* temp,http_request* httpRequest)
{
    /* 遇到一个空行,说明我们得到了一个正确的请求 */
    if( temp[ 0 ] == '\0' )
    {
        return GET_REQUEST;
    }
    else if( isHeaderKey("Host:")) /* 处理"host"头部字段 */
    {
        temp += sizeof("Host:")-1;
        temp += strspn( temp, " \t" );
        printf( "the request host is: %s\n", temp );
    }
    else /* 其他头部字段都不处理 */
    {
        printf( "I can not handle this header\n" );
    }
    return NO_REQUEST;
}

/* 分析http请求 的入口函数 */
HTTP_CODE http_parse_content( char* buffer, int* checked_index, CHECK_STATE*
checkstate, int* read_index, int* start_line,http_request* httpRequest)
{
    LINE_STATUS linestatus = LINE_OK; //记录当前行的读取状态
    HTTP_CODE retcode = NO_REQUEST; //记录http请求的处理结果
    /* 主状态机 用于从buffer中取出所有完整的行 */
    while( ( linestatus = http_parse_line( buffer, checked_index, read_index,httpRequest ) ) == LINE_OK )
    {
        char* temp = buffer + *start_line; //start_line是行在buffer中的起始位置
        *start_line = *checked_index;     //记录下一行的起始位置
        /* checkstate 记录主状态机当前的状态 */
        switch ( *checkstate )
        {
            case CHECK_STATE_REQUESTLINE: //第一个状态,分析请求行
            {
                retcode = http_parse_requestline( temp, checkstate ,httpRequest);
                if( retcode == BAD_REQUEST )
                {
                    return BAD_REQUEST;
                }
                break;
            }
            case CHECK_STATE_HEADER:      //第二个状态，分析头部字段
            {
                retcode = http_parse_headers( temp,httpRequest);
                if( retcode == BAD_REQUEST )
                {
                    return BAD_REQUEST;
                }
                else if ( retcode == GET_REQUEST )
                {
                    return GET_REQUEST;
                }
                break;
            }
            default:
            {
                return INTERNAL_ERROR;
            }
        }
    }
    /* 如果没有读取到一个完整的行,则表示还需要继续读取客户数据才能进一步分析 */
    if( linestatus == LINE_OPEN )
    {
        return NO_REQUEST;
    }
    else
    {
        return BAD_REQUEST;
    }
}

#endif //PROCESSPOOL_HTTP_PASER_H
