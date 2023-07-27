#include "connection.h"
#include "common.h"

int create_socket(void)
{
    int listenfd = socket(AF_INET6, SOCK_STREAM, 0);

    if (listenfd < 0) {
        fatal_error("socket creation error!");
    }

    // Make the socket reuse port and address

    const int enable = 1;
    int err_num = 0;
    err_num = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    if (err_num < 0) {
        fatal_error("setsockopt(SO_REUSEADDR) failed");
    }

    err_num = setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));

    if (err_num < 0) {
        fatal_error("setsockopt(SO_REUSEPORT) failed");
    }

    return listenfd;
}

struct sockaddr_in6 setup_addr(int port)
{
    struct sockaddr_in6 serve_address;
    memset(&serve_address, 0, sizeof(serve_address));

    serve_address.sin6_family = AF_INET6;
    serve_address.sin6_addr = in6addr_any;
    serve_address.sin6_port = htons(port);

    return serve_address;
}

void bind_and_listen(struct Connection* conn)
{
    int err_num = 0;

    err_num = bind(
        conn->listenfd,
        (struct sockaddr*) &conn->serve_address,
        sizeof(conn->serve_address)
    );

    if (err_num < 0) {
        fatal_error("bind error");
    }

    err_num = listen(conn->listenfd, 10);

    if (err_num < 0) {
        fatal_error("listen error");
    }
}

struct Connection connection_new(int port) 
{
    struct Connection conn;

    conn.listenfd = create_socket();
    conn.serve_address = setup_addr(port);

    bind_and_listen(&conn);

    return conn;
}

struct Request* connection_get_request(struct Connection* conn)
{
    struct Request* req = malloc(sizeof(struct Request));

    req->connfd = accept(
        conn->listenfd,
        (struct sockaddr*) NULL,
        NULL
    );


    memset(req->buffer, 0, REQUEST_MAX_SIZE);

    // Read the request into buffer
    req->size = read(req->connfd, req->buffer, REQUEST_MAX_SIZE - 1);

    if (req->size < 0) {
        fatal_error("connection read error");
    }

    return req;
}


void connection_delete_request(struct Request* req)
{
    close(req->connfd);
    free(req);
}
