#include "../inc/network.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>



// Function Name: connectToServer
// Parameters:
//     const char *server_name - The server hostname or IP address.
//     int port                - The server port number.
// Return Value:
//     int - The connected socket file descriptor.
// Description:
//     Establishes a TCP connection to the specified server and port.
//     Resolves the server hostname, creates a socket, and connects to it.
//     On failure, prints an error and exits the program.

int connectToServer(const char *serverName, int port) 
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;


    // Create TCP socket (IPv4, stream-based)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    // Resolve server name to IP address
    server = gethostbyname(serverName);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR: no such host %s\n", serverName);
        exit(EXIT_FAILURE);
    }


    // Fill in server address struct
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);


    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}



// Function Name: sendUserId
// Parameters:
//     int sockfd         - The connected socket descriptor.
//     const char *user_id - The client's user ID string.
// Return Value: None
// Description:
//     Sends the user's ID string to the server over the connected socket.
//     On failure, prints an error and exits the program.
void sendUserId(int sockfd, const char *userId) 
{
    if (send(sockfd, userId, strlen(userId), 0) < 0) 
    {
        perror("send user_id");
        exit(EXIT_FAILURE);
    }
}