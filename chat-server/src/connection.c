#include "../inc/connection.h"
#include "../inc/clientHandler.h"
#include "../inc/clientList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for close()
#include <pthread.h>
#include <netinet/in.h>  // for sockaddr_in, INADDR_ANY
#include <arpa/inet.h>  // for htons(), inet_addr()



// Function Name: startServer
// Parameters:
//     int port - The port number on which the server should listen for client connections.
// Return Value: None
// Description:
//     Starts a TCP server that listens for incoming client connections on the specified port.
//     It creates a listening socket, binds it to the address, and accepts connections in a loop.
//     For each new client, it creates a detached thread to handle communication using handleClient().
//     If the maximum number of allowed clients is reached, new connections are rejected.
//     Thread-safe operations are ensured via supporting client list functions (e.g., getClientCount).

void startServer(int port)
{
    int serverfd = 0; // socket file descriptorn of monitor
    int clientSocket = 0; // client socket
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);



    // Create sockets (IPv4 + TCP)
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }


    // Configure server address information
    address.sin_family = AF_INET;          // Use IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Accept any source IP address
    address.sin_port = htons(port);        // Set up listening ports (converted to network byte order)


    // Bind the socket to the specified IP and port.
    if (bind(serverfd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind Failed to bind");
        exit(EXIT_FAILURE);
    }


    // Start listening for connection requests
    if (listen(serverfd, MAX_CLIENTS) < 0) 
    {
        perror("listen Listening failed.");
        exit(EXIT_FAILURE);
    }


    printf("[SERVER Debug] Listening on a port. %d...\n", port);


    // Loop through client connections
    while(1)
    {
        clientSocket = accept(serverfd, (struct sockaddr *)&address, &addrlen);
        if (clientSocket < 0) 
        {
            perror("accept Receive failure");
            continue;
        }


        // Reject new connections if the maximum number of connections is exceeded
        if (getClientCount() >= MAX_CLIENTS) 
        {
            printf("[SERVER Debug] Maximum number of connections reached, new connections denied.\n");
            close(clientSocket);
            continue;
        }


        // Allocate memory for passing client sockets to threads
        int *pclient = malloc(sizeof(int));
        if (pclient == NULL) 
        {
            perror("Memory allocation failure");
            close(clientSocket);
            continue;
        }


        *pclient = clientSocket;


        // Create a thread to process the client
        pthread_t tid;
        if (pthread_create(&tid, NULL, handleClient, pclient) != 0)
        {
            perror("pthread_create Failed to create a thread.");
            close(clientSocket);
            free(pclient);
        }
        else 
        {
            // Separate threads, no need to manually recycle
            pthread_detach(tid);
        }
    }
}