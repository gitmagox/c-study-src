//
// Created by magox on 7/7/20.
//

#ifndef PROCESSPOOL_HTTP_H
#define PROCESSPOOL_HTTP_H

#include <hash_map.h>
#include <Interface/protocol.h>
#include <Protocol/http_paser.h>

enum HTTP_METHOD {OPTIONS,GET,HEAD,POST,PUT,DELETE,TRACE,CONNECT};

static const char *http_method_strings[] ={
                "OPTIONS","GET", "HEAD" , "POST", "PUT" , "DELETE" , "TRACE", "CONNECT"
};

enum STRTOK {CR,LF,SP,CRLF};
static const char *strtok_string[] ={"\r","\n", " " , "\r\n"};

#define http_strtok(T) &strtok_string[T]
#define http_method_string(T) &http_method_strings[T]

typedef struct http_protocol {
    struct ProtocolInterface protocolInterface;
    request_mannege * requestMannege;
} http_protocol;


static inline int http_input(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn);
static inline int http_encode(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn);
static inline void * http_decode(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn);

static inline int http_input(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn)
{
    ProtocolMessage *request_message = conn->get_protocol_message(conn);
    int data_read = 0;
    int * read_index = &(request_message->read_index); //当前已经读取了多少字节的客户数据
    int * checked_index = &(request_message->checked_index); //当前已经分析完了多少字节的客户数据
    int * start_line = &(request_message->start_line); //行在buffer中的地始位置
    /* 设置主状态机的初始状态 */
    CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;
    data_read = recv( conn->fd, buffer + *read_index, 65535 - *read_index, 0 );
    if ( data_read == 0 ){
        printf( "reading failed\n" );
        return MESSAGE_READ_NOTHING;
    }
    read_index += data_read;
    /* 分析目前已经取得的所有客户数据 */
    HTTP_CODE result = http_parse_content( buffer,checked_index, &checkstate, read_index, start_line );
    return result;
}

static inline int http_encode(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn){

}

static inline void * http_decode(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn){
    ProtocolMessage *message = conn->get_protocol_message(conn);
    http_request *httpRequest = get_thiz_parent(http_request, protocolMessage, message);
    return httpRequest;
}

//get Singleton
static inline http_protocol *http_protocol_create(){
    static http_protocol * httpProtocol = NULL;
    //next edit;
    if(NULL == httpProtocol){
        httpProtocol = (http_protocol *)malloc( sizeof(http_protocol));
        memset(httpProtocol,"\0",sizeof(request_mannege ));
        httpProtocol->requestMannege = create_request_mannege();
        httpProtocol->protocolInterface.input=http_input;
        httpProtocol->protocolInterface.encode=http_encode;
        httpProtocol->protocolInterface.decode=http_decode;
    }
    return httpProtocol;
}

#endif //PROCESSPOOL_HTTP_H
