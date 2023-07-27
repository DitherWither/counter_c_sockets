#include "common.h"
#include "connection.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    struct Connection conn = connection_new(8080);

    while (true) {
        struct Request* req = connection_get_request(&conn);

        char* buffer = malloc(1024);

        sprintf(buffer, "HTTP/1.0 200 OK\r\n");
        sprintf(buffer, "\r\n"); // no headers
        sprintf(buffer, "Hello, World!");

        write(req->connfd, buffer, strlen(buffer));

        connection_delete_request(req);
    }
}
