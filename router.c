#include "common.h"
#include "router.h"
#include "connection.h"
#include <stdio.h>
#include <string.h>

void index_handler(struct ParsedRequest* req, char* res);
void not_found_handler(struct ParsedRequest* req, char* res);
char *str_replace(char *orig, char *rep, char *with);

int count = 0;

void router_handle_request(struct Request* raw_req)
{
    struct ParsedRequest* req = parse_request(raw_req);

    char* buffer = malloc(RESPONSE_MAX_SIZE);
    memset(buffer, 0, RESPONSE_MAX_SIZE);

    if (strcmp("/", req->path) == 0) {
        index_handler(req, buffer);
        goto end_routing;
    }

    not_found_handler(req, buffer);
    
end_routing:
    write(raw_req->connfd, buffer, strlen(buffer));

    free(buffer);
    free(req);
}

char* index_body = NULL;

void not_found_handler(struct ParsedRequest* req, char* res)
{
    sprintf(res, "HTTP/1.1 200 OK\r\n");
    sprintf(res + strlen(res), "Content-Type:text/html\r\n");

    char body[4096];

    sprintf(body, "<h1>Not Found</h1>");

    print_body(res, body);
}

void load_index(void)
{
    FILE *f = fopen("index.html", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    index_body = malloc(fsize + 1);
    fread(index_body, fsize, 1, f);
    fclose(f);

    index_body[fsize] = 0;
}

void index_handler(struct ParsedRequest* req, char* res) 
{   
    if (strcmp(req->method, "POST") == 0) {
        count++;
    }

    sprintf(res, "HTTP/1.1 200 OK\r\n");
    sprintf(res + strlen(res), "Content-Type:text/html\r\n");
    
    if (index_body == NULL) {
        load_index();
    }

    char count_str[8];
    sprintf(count_str, "%d", count);

    char* body = str_replace(index_body, "{{count}}", count_str);
    print_body(res, body);
}


struct ParsedRequest* parse_request(struct Request* raw_req)
{
    struct ParsedRequest* req = malloc(sizeof(struct ParsedRequest));

    char* headers_start = 0;
    

    sscanf(raw_req->buffer, "%8s %256s %16s", req->method, req->path, req->http_version);

    // This code is utter garbage lol
    for (int i = 4; i < strlen(raw_req->buffer); i++) {
        if (raw_req->buffer[i - 1] == '\n' && raw_req->buffer[i - 2] == '\r') {
            if (raw_req->buffer[i - 3] == '\n' && raw_req->buffer[i - 4] == '\r') {
                // If is end of header
                req->body = &raw_req->buffer[i];
                break;
            }
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


// This code was copied from SO, as I can't be bothered
// with this lol
char *str_replace(char *orig, char *rep, char *with) 
{
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
