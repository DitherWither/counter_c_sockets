#include "common.h"
#include "connection.h"
#include "router.h"

int main(int argc, char** argv)
{
    struct Connection conn = connection_new(8080);

    while (true) {
        struct Request* req = connection_get_request(&conn);

        router_handle_request(req);

        connection_delete_request(req);
    }
}

