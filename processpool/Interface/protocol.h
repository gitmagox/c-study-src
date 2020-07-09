//
// Created by magox on 7/7/20.
//

#ifndef PROCESSPOOL_PROTOCOL_H
#define PROCESSPOOL_PROTOCOL_H

#include "base.h"
#include "connection.h"

struct ProtocolInterface;
typedef struct ProtocolInterface ProtocolInterface;

struct ProtocolInterface
{
    int (*input)(ProtocolInterface  *thiz,char* buffer,ConnectionInterface *conn);
    int (*encode)(ProtocolInterface *thiz,char* buffer,ConnectionInterface *conn);
    void * (*decode)(ProtocolInterface *thiz,char* buffer,ConnectionInterface *conn);
};


typedef struct ProtocolMessage ProtocolMessage;

struct ProtocolMessage
{
    int fd;
    char * buffer;
    uint32_t read_index;
    uint32_t checked_index;
    uint32_t start_line;
};

/*
 *messge协议input后的结果:
 *NO_REQUEST 表示请求不完整，需要继续读取客户数据;
 *GET_REQUEST 表示取得了一个完整的客户请求;
 *BAD_REQUEST 表示客户请求有语法错误;
 *FORBIDDEN_REQUEST 表示客户对资源没有足够的访问权限;
 *INTERNAL_ERROR 表示服务器内部错误;
 *CLOSED_CONNECTION 表示客户端已经关闭连接了;
 */
typedef enum {
    MESSAGE_NO_REQUEST,
    MESSAGE_GET_REQUEST,
    MESSAGE_BAD_REQUEST,
    MESSAGE_FORBIDDEN_REQUEST,
    MESSAGE_INTERNAL_ERROR,
    MESSAGE_CLOSED_CONNECTION,
    MESSAGE_READ_NOTHING
}MESSAGE_STATUS_CODE;


static inline int input(ProtocolInterface *thiz, char* buffer,void * connection)
{
    if(thiz->input != NULL)
    {
        return thiz->input(thiz,buffer, connection);
    }
    return RET_OK;
}

static inline int encode(ProtocolInterface *thiz, char* buffer,void * connection)
{
    if(thiz->encode != NULL)
    {
        return thiz->encode(thiz,buffer, connection);
    }
    return RET_OK;
}

static inline void * decode(ProtocolInterface *thiz, char* buffer,void * connection)
{
    if(thiz->decode != NULL)
    {
        return thiz->decode(thiz,buffer, connection);
    }
    return NULL;
}

#endif //PROCESSPOOL_PROTOCOL_H
