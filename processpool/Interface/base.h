//
// Created by magox on 7/8/20.
//

#ifndef PROCESSPOOL_BASE_H
#define PROCESSPOOL_BASE_H

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/stat.h>

enum
{
    RET_OK,
    RET_FAIL
};

#define get_thiz_parent(P,T,Z) (P *)((char *) Z + offsetof( P, T ))
#define get_thiz(P,T,t,B) (T *)((char *) B - offsetof( P, t ))


/**
 * timer
 */
struct TimerInterface;
typedef struct TimerInterface TimerInterface;

struct TimerInterface
{
    int (*add)(TimerInterface *thiz,int timeout,int fd,void(*handler)(int, void* ),void * args);
    int (*del)(TimerInterface *thiz,void *timer);
    int (*start)(TimerInterface *thiz);
};

/**
 * ProtocolMessage
 */
typedef struct ProtocolMessage ProtocolMessage;

struct ProtocolMessage
{
    int fd;
    char * buffer;
    uint32_t read_index;
    uint32_t checked_index;
    uint32_t start_line;
    int (*destroy)(ProtocolMessage *thiz);
};

enum {
    HTTP_PROTOCOL
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

/**
 * ConnectionInterface
 */
struct ConnectionInterface;
typedef struct ConnectionInterface ConnectionInterface;

struct ConnectionInterface
{
    int fd;
    int (*send)(ConnectionInterface *thiz, char* buffer);
    ProtocolMessage * (*get_protocol_message)(ConnectionInterface *thiz);
    int  (*set_protocol_message)(ConnectionInterface *thiz,ProtocolMessage * protocolMessage);
    int (*close)(ConnectionInterface *thiz,char* message);
    int (*destroy)(ConnectionInterface *thiz);
};

/**
 * ProtocolInterface
 */
struct ProtocolInterface;
typedef struct ProtocolInterface ProtocolInterface;

struct ProtocolInterface
{
    int (*input)(ProtocolInterface  *thiz,char* buffer,ConnectionInterface *conn);
    int (*encode)(ProtocolInterface *thiz,char* buffer,ConnectionInterface *conn);
    void * (*decode)(ProtocolInterface *thiz,char* buffer,ConnectionInterface *conn);
};


#endif //PROCESSPOOL_BASE_H
