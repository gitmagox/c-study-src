//
// Created by magox on 7/7/20.
//

#ifndef PROCESSPOOL_TCP_CONNECTION_H
#define PROCESSPOOL_TCP_CONNECTION_H

#include "hash_map.h"
#include "select_event.h"
#include "Interface/connection.h"
#include "Protocol/protocol_factory.h"

/**
 * tcp connection
 */
const READ_BUFFER_SIZE = 65535;

enum {
    TCP_STATUS_INITIAL=0,
    TCP_STATUS_CONNECTING=1,
    TCP_STATUS_ESTABLISHED=2,
    TCP_STATUS_CLOSING=4,
    TCP_STATUS_CLOSED=8,
};

typedef struct connection_mannege connection_mannege;

typedef struct tcp_connection{
    struct ConnectionInterface connectionInterface;
    connection_mannege * connectionMannege;
    ProtocolMessage * request_data;
    int id;
    int fd;
    int status:TCP_STATUS_ESTABLISHED;
    char * recv_buffer;
    uint32_t _currentPackageLength;
    int protocol:1;
    uint32_t  bytesRead;
    uint32_t maxPackageSize;
    uint32_t currentPackageLength;
    char * send_buffer;
    uint32_t sendBufferSize;
    void (*onMessage)(int,void*);
} tcp_connection;

typedef map_t(tcp_connection * ) map_tcp_connection_t;

typedef struct connection_mannege{
    map_tcp_connection_t * connections;
    int counts;
} connection_mannege;

static inline void baseRead(int fd,tcp_connection *tcpConnection){
    char buffer[tcpConnection->maxPackageSize];
    memset( buffer, '\0', tcpConnection->maxPackageSize );
    if(tcpConnection->request_data==NULL){
        tcpConnection->request_data = build_message(1,fd,&tcpConnection->connectionInterface, buffer);
    }
    while (1){
        //protocol_input
        ProtocolInterface * protocol = build_protocol(tcpConnection->protocol);
        MESSAGE_STATUS_CODE result = protocol->input(protocol,tcpConnection->recv_buffer,tcpConnection);
        if ( result == MESSAGE_NO_REQUEST ){//数据包不是完整的继续分析
            continue;
        } else if (result==MESSAGE_READ_NOTHING){//没有读到数据
            break;
        }else if (result == MESSAGE_GET_REQUEST){//得到一个完整的请求
            if(tcpConnection->onMessage=NULL){
                continue;
            }
            //callback onMessage;
            tcpConnection->onMessage(tcpConnection,protocol->decode(protocol,buffer,&tcpConnection->connectionInterface));
            break;
        }else{
            break;
        }
    }
}


static inline void recv_buffer_read(int fd, void* tcp_connection){
    baseRead(fd,tcp_connection);
}

static inline connection_mannege * create_connection_mannege(){
    static connection_mannege *connectionMannege=NULL;
    if(connectionMannege==NULL){
        map_tcp_connection_t * connections;
        connectionMannege = (connection_mannege *)malloc( sizeof(connection_mannege));
        connections = (map_tcp_connection_t *)malloc( sizeof(map_tcp_connection_t));
        memset(connectionMannege,"\0",sizeof(connection_mannege ));
        memset(connections,"\0",sizeof(map_tcp_connection_t ));
        hash_map_init(connections);
        connectionMannege->connections = connections;
        connectionMannege->counts=0;
    }
    return connectionMannege;
}

static inline int connection_mannage_add(connection_mannege * connectionMannege,tcp_connection * connection){
    char * key = hash_map_get_key(int,1,connection->fd);
    hash_map_set(connectionMannege->connections,key,connection);
    hash_map_free_key(key);
    connectionMannege->counts ++;
    return RET_OK;
}

static inline tcp_connection * connection_mannage_get(connection_mannege * connectionMannege,int fd){
    char * key = hash_map_get_key(int,1,fd);
    tcp_connection ** pTcpConnection = hash_map_get(connectionMannege->connections,key);
    return (*pTcpConnection);
}

static inline int tcp_connection_destroy(ConnectionInterface *thiz){
    tcp_connection * tcpConnection = get_thiz(tcp_connection,ConnectionInterface,connectionInterface,thiz);
    char * key = hash_map_get_key(int,1,tcpConnection->fd);
    hash_map_remove(tcpConnection->connectionMannege->connections,key);
    hash_map_free_key(key);
    free(tcpConnection);
    return RET_OK;
}

static inline int tcp_connection_send(ConnectionInterface *thiz, char* buffer){
    return RET_OK;
}

static inline int tcp_connection_close(ConnectionInterface *thiz){
    return RET_OK;
}

ProtocolMessage * tcp_connection_get_protocol_message(ConnectionInterface *thiz){
    tcp_connection* tcpConnection = get_thiz_parent(tcp_connection,connectionInterface,thiz);
    ProtocolMessage * protocolMessage = tcpConnection->request_data;
    return protocolMessage;
}


static inline tcp_connection * new_tcp_connection(connection_mannege * connectionMannege,int fd,int protocol,void(*onMessage)(void * connection,void * request),select_event * selectEvent){
    tcp_connection * tcpConnection;
    tcpConnection = (tcp_connection *)malloc( sizeof(tcp_connection));
    memset(tcpConnection,"\0",sizeof(tcp_connection ));
    tcpConnection->id = connectionMannege->counts+1;
    tcpConnection->fd = fd;
    tcpConnection->request_data = NULL;
    tcpConnection->protocol = protocol;
    tcpConnection->currentPackageLength = 0;
    tcpConnection->maxPackageSize =1048576;
    tcpConnection->sendBufferSize = 1048576;
    tcpConnection->bytesRead=0;
    tcpConnection->onMessage = onMessage;
    tcpConnection->connectionInterface.send = tcp_connection_send;
    tcpConnection->connectionInterface.close = tcp_connection_close;
    tcpConnection->connectionInterface.destroy = tcp_connection_destroy;
    tcpConnection->connectionInterface.get_protocol_message = tcp_connection_get_protocol_message;
    tcpConnection->connectionInterface.fd = fd;
    connection_mannage_add(connectionMannege,tcpConnection);
    select_event_add(selectEvent,fd,EPOLLIN,recv_buffer_read,tcpConnection);
    tcpConnection->connectionMannege = connectionMannege;
    return tcpConnection;
}


#endif //PROCESSPOOL_TCP_CONNECTION_H
