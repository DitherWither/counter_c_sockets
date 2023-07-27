#ifndef _ROUTER_H_
#define _ROUTER_H_

#include "common.h"
#include "connection.h"

#define RESPONSE_MAX_SIZE 8192

void router_handle_request(struct Request*);

#endif
