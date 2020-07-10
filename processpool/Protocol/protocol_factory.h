//
// Created by magox on 7/8/20.
//
#ifndef PROCESSPOOL_PROTOCOL_FACTORY_H
#define PROCESSPOOL_PROTOCOL_FACTORY_H
#include "http.h"

static inline ProtocolInterface * build_protocol(int type){
    ProtocolInterface * protocol;
    if(type==HTTP_PROTOCOL){
        http_protocol * httpProtocol = http_protocol_create();
        protocol = get_thiz(http_protocol,ProtocolInterface,protocolInterface,httpProtocol);
    }
    return protocol;
};

static inline ProtocolMessage * build_message(int type,int fd,ConnectionInterface *conn,char * buffer){
    ProtocolMessage * protocolMessage;
    if(type==HTTP_PROTOCOL){
        http_request * httpProtocol = new_http_request(fd,conn,buffer);
        protocolMessage = get_thiz(http_protocol,ProtocolInterface,protocolInterface,httpProtocol);
    }
    return protocolMessage;
}

#endif //PROCESSPOOL_PROTOCOL_FACTORY_H
