#ifndef _ROUTER_H_
#define _ROUTER_H_

#include "common.h"
#include "connection.h"

#define RESPONSE_MAX_SIZE 8192

struct ParsedRequest {
    char path[256];
    char method[8];
    char http_version[16];
    char* headers[64];
    char* body;
};


void router_handle_request(struct Request*);

#endif
