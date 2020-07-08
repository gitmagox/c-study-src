//
// Created by magox on 7/7/20.
//

#ifndef PROCESSPOOL_HTTP_H
#define PROCESSPOOL_HTTP_H

#include "../Interface/protocol.h"

typedef struct http_protocol {
    struct ProtocolInterface protocolInterface;
} http_protocol;

static inline http_protocol *http_protocol_create(){
    http_protocol * httpProtocol;
    //next edit;

    return httpProtocol;
}


#endif //PROCESSPOOL_HTTP_H
