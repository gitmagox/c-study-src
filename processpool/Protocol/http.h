//
// Created by magox on 7/7/20.
//

#ifndef PROCESSPOOL_HTTP_H
#define PROCESSPOOL_HTTP_H

#include <hash_map.h>
#include <Interface/protocol.h>
#include <Protocol/http_paser.h>



typedef struct http_protocol {
    struct ProtocolInterface protocolInterface;
    request_mannege * requestMannege;
} http_protocol;


static inline int http_input(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn);
static inline int http_encode(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn);
static inline void * http_decode(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn);

static inline int http_input(ProtocolInterface *thiz, char* buffer,ConnectionInterface * conn)
{
    http_request *httpRequest;
    ProtocolMessage *request_message = conn->get_protocol_message(conn);
    if(request_message==NULL){
        request_mannege * requestMannege =  create_request_mannege();
        httpRequest = new_http_request(conn->fd,conn, buffer);
        int ret = request_mannage_add(requestMannege,httpRequest);
        if(ret!=RET_OK){
            return MESSAGE_BAD_REQUEST;
        }
        request_message = &httpRequest->protocolMessage;
        conn->set_protocol_message(conn,request_message);
    }else{
        httpRequest = request_mannage_get(request_message);
    }
    int data_read = 0;
    /* 设置主状态机的初始状态 */
    CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;
    HTTP_CODE result;
    while(1){

        do{
            data_read = recv( conn->fd, buffer + request_message->read_index, 65535 - request_message->read_index, 0 );
        }while (data_read < 0 && errno == EINTR);

        if ( data_read == 0 ){
            printf( "reading failed\n" );
            return MESSAGE_READ_NOTHING;
        }
        if( data_read <0){
            char *sendbuffer;
            sendbuffer="HTTP/1.1 200 OK\r\n"\
                       "Server: swoole-http-server\r\n"\
                       "Connection: keep-alive\r\n"\
                       "Content-Type: text/html\r\n"\
                       "Date: Thu, 18 Jun 2020 06:16:24 GMT\r\n"\
                       "Content-Length: 0\r\n"\
                       "Set-Cookie: PHPSESSID=kpdq480qjchb6gunnjsgd3lnnl; path=/\r\n";
            conn->close(conn,sendbuffer);
            break;
        }
        request_message->read_index += data_read;
        /* 分析目前已经取得的所有客户数据 */
        result = http_parse_content( buffer,&(request_message->checked_index), &checkstate, &(request_message->read_index), &(request_message->start_line) ,httpRequest);
        if ( result == NO_REQUEST ){//数据包不是完整的继续分析
            continue;
        }else if(result == BAD_REQUEST){//失败的请求,//关闭链接
            char *sendbuffer;
            sendbuffer="HTTP/1.1 200 OK\r\n"  \
                       "Server: swoole-http-server\r\n"\
                       "Connection: keep-alive\r\nContent-Type: text/html\r\n"\
                       "Date: Thu, 18 Jun 2020 06:16:24 GMT\r\n"\
                       "Content-Length: 0\r\n"\
                       "Set-Cookie: PHPSESSID=kpdq480qjchb6gunnjsgd3lnnl; path=/\r\n\r\n";
            conn->close(conn,sendbuffer);
        }
        break;
    }
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
