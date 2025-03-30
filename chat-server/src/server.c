#include "../inc/server.h"
#include "../inc/clientList.h"
#include "../inc/connection.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int port = SERVER_PORT;

    if (argc == 2)
    {
        port = atoi(argv[1]);
    }

    printf("[SERVER DEBUG] Startig server on port %d\n", port);

    initClientList();
    startServer(port);

    return 0;
}