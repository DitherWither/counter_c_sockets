#ifndef _SOCKETMGR_H_
#define _SOCKETMGR_H_

#include "common.h"

#define REQUEST_MAX_SIZE 8192

struct Request {
    uint8_t buffer[REQUEST_MAX_SIZE + 1];
    ssize_t size;
    int connfd;
};

struct Connection {
    int listenfd;

    struct sockaddr_in6 serve_address;
};

/// Creates a new connection struct, and binds
/// and listens to the port in args
struct Connection connection_new(int port);

/// Waits for a request to be made on the server, and
/// then returns a pointer to the buffer storing 
/// the raw request string
struct Request* connection_get_request(struct Connection* conn);

void connection_delete_request(struct Request* req);

#endif
