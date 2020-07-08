//
// Created by magox on 7/8/20.
//

#ifndef PROCESSPOOL_PROTOCOL_FACTORY_H
#define PROCESSPOOL_PROTOCOL_FACTORY_H

#include "http.h"


enum {
    HTTP_PROTOCOL = 1
};

static inline ProtocolInterface * build_protocol(int type){
    ProtocolInterface * protocol;
    if(type==HTTP_PROTOCOL){
        http_protocol * httpProtocol = http_protocol_create();
        protocol = get_thiz(http_protocol,ProtocolInterface,protocolInterface,httpProtocol);
    }
    return protocol;
}
#endif //PROCESSPOOL_PROTOCOL_FACTORY_H
