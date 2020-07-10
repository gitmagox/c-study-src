//
// Created by magox on 7/9/20.
//

#ifndef PROCESSPOOL_REQUEST_H
#define PROCESSPOOL_REQUEST_H

#include "hash_map.h"

/**
 * http
 */

typedef struct request_mannege request_mannege;

typedef struct http_request{
    struct ProtocolMessage protocolMessage;
    uint32_t id;
    int fd;
    ConnectionInterface * conn;
    char * buffer;
    request_mannege * requestMannege;
    uint32_t start_line;
} http_request;


typedef map_t(http_request * ) map_http_request_t;

typedef struct request_mannege{
    map_http_request_t * requests;
    int counts;
} request_mannege;



static inline request_mannege * create_request_mannege();
static inline int request_mannage_add(request_mannege * connectionMannege,http_request * httpRequest);
static inline int request_mannage_remove(request_mannege * connectionMannege,http_request * httpRequest);
static inline int destroy_http_request(http_request * httpRequest);
static inline http_request * new_http_request();

static inline request_mannege * create_request_mannege(){
    static request_mannege *requestMannege = NULL;
    if(requestMannege==NULL){
        map_http_request_t * connections;
        requestMannege = (request_mannege *)malloc( sizeof(request_mannege));
        connections = (map_http_request_t *)malloc( sizeof(map_http_request_t));
        memset(requestMannege,"\0",sizeof(request_mannege ));
        memset(connections,"\0",sizeof(map_http_request_t ));
        hash_map_init(connections);
        requestMannege->requests = connections;
        requestMannege->counts=0;
    }
    return requestMannege;
}

static inline int request_mannage_add(request_mannege * connectionMannege,http_request * httpRequest){
    char * key = hash_map_get_key(int,1,httpRequest->id);
    hash_map_set(connectionMannege->requests,key,httpRequest);
    hash_map_free_key(key);
    httpRequest->requestMannege= connectionMannege;
    connectionMannege->counts ++;
    return RET_OK;
}

static inline int request_mannage_remove(request_mannege * connectionMannege,http_request * httpRequest){
    char * key = hash_map_get_key(int,1,httpRequest->id);
    hash_map_remove(connectionMannege->requests,key);
    hash_map_free_key(key);
    destroy_http_request(httpRequest);
    connectionMannege->counts --;
    return RET_OK;
}

static inline http_request * request_mannage_get(request_mannege * connectionMannege,ProtocolMessage * message) {
    http_request *httpRequest = get_thiz_parent(http_request, protocolMessage, message);
    return httpRequest;
}

static inline http_request * new_http_request(int fd,ConnectionInterface *conn, char *buffer){
    http_request * httpRequest;
    httpRequest = (http_request *)malloc( sizeof(http_request));
    memset(httpRequest,"\0",sizeof(http_request ));
    httpRequest->fd = fd;
    httpRequest->protocolMessage.fd = fd;
    httpRequest->conn = conn;
    httpRequest->buffer = buffer;
    httpRequest->protocolMessage.buffer = buffer;
    httpRequest->protocolMessage.checked_index=0;
    httpRequest->protocolMessage.read_index=0;
    httpRequest->start_line=0;
    request_mannege * requestMannege = create_request_mannege();
    request_mannage_add(requestMannege,httpRequest);
    httpRequest->id = requestMannege->counts;
    return httpRequest;
}

static inline int destroy_http_request(http_request * httpRequest){
    request_mannege * requestMannege = create_request_mannege();
    request_mannage_remove(requestMannege,httpRequest);
    free(httpRequest);
    httpRequest = NULL;
    return RET_OK;
}

#endif //PROCESSPOOL_REQUEST_H
