#include "common.h"
#include "router.h"
#include "connection.h"
#include <stdlib.h>
#include <string.h>

struct ParsedRequest* parse_request(struct Request* raw_req);
void print_body(char* buffer, char* body);

void router_handle_request(struct Request* raw_req)
{
    struct ParsedRequest* req = parse_request(raw_req);

    char* buffer = malloc(RESPONSE_MAX_SIZE);
    memset(buffer, 0, RESPONSE_MAX_SIZE);

    sprintf(buffer, "HTTP/1.1 200 OK\r\n");
    sprintf(buffer + strlen(buffer), "Content-Type:text/html\r\n");


    char body[4096];
    sprintf(body, "<h1>Hello at %s using %s</h1>", req->path, req->method);
    sprintf(body + strlen(body), "<p> Body was: %s </p>", req->body);
    sprintf(body + strlen(body), "<p> The first header was: %s </p>", req->headers[0]);
    print_body(buffer, body);

    write(raw_req->connfd, buffer, strlen(buffer));

    free(buffer);
    free(req);
}

struct ParsedRequest* parse_request(struct Request* raw_req)
{
    struct ParsedRequest* req = malloc(sizeof(struct ParsedRequest));

    char* headers_start = 0;
    char* current_line_start = 0;
    

    sscanf(raw_req->buffer, "%8s %256s %16s", req->method, req->path, req->http_version);

    // This code is utter garbage lol
    for (int i = 2; i < strlen(raw_req->buffer); i++) {
        if (raw_req->buffer[i - 1] == '\n' && raw_req->buffer[i - 2] == '\r') {
            if (raw_req->buffer[i - 3] == '\r' && raw_req->buffer[i - 4] == '\n') {
                // If is end of header
                printf("i was %d", i);
                req->body = &raw_req->buffer[i];
                break;
            }
            current_line_start = &raw_req->buffer[i];
            if (headers_start == 0) {
                headers_start = &raw_req->buffer[i];
            }
        }
    }

    char* token;
    int i = 0;
    while ((token = strsep(&headers_start, "\r\n"))) {
        req->headers[i] = token;
        i++;
    }

    return req;
}


void print_body(char* buffer, char* body)
{
    int len = strlen(body);

    sprintf(buffer + strlen(buffer), "Content-Length:%d\r\n", len);
    sprintf(buffer + strlen(buffer), "\r\n");
    sprintf(buffer + strlen(buffer), "%s", body);
}
