#include "common.h"
#include "router.h"
#include "connection.h"

void router_handle_request(struct Request* req)
{
    char* buffer = malloc(RESPONSE_MAX_SIZE);
    char* path_buffer = malloc(256);
    char* method_buffer = malloc(8);
    char* http_version = malloc(16);

    memset(buffer, 0, RESPONSE_MAX_SIZE);

    sscanf(req->buffer, "%8s %256s %16s", method_buffer, path_buffer, http_version);

    sprintf(buffer, "HTTP/1.1 200 OK\r\n");
    sprintf(buffer, "\r\n");
    sprintf(buffer, "Hello at %s using %s", path_buffer, method_buffer);

    write(req->connfd, buffer, strlen(buffer));

    free(buffer);
    free(path_buffer);
    free(method_buffer);
    free(http_version);
}
