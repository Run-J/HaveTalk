#include "../inc/client.h"
#include "../inc/network.h"
#include "../inc/ui.h"
#include "../inc/receiver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#define DEFAULT_PORT 13000



// Function Name: main
// Parameters:
//     int argc      - The number of command-line arguments.
//     char *argv[]  - The list of command-line arguments.
// Return Value:
//     int - Exit status: 0 for success, non-zero for failure.
// Description:
//     Parses command-line arguments for username and server hostname.
//     Connects to the server via TCP, sends the user ID, and prepares
//     for further communication (UI, receiving thread – to be implemented).
int main(int argc, char *argv[]) 
{
    ClientConfig config;
    memset(&config, 0, sizeof(config));  // Initialize the configuration structure


    // Parsing command line arguments, e.g. /chat-client -userRunJi -server127.0.0.1
    for (int i = 1; i < argc; i++) 
    {
        if (strncmp(argv[i], "-user", 5) == 0)  // compare the first five chars
        {
            // Copy the username to config.userId
            strncpy(config.userId, argv[i] + 5, MAX_NAME_LEN - 1); // skip the first five chars
        } 
        else if (strncmp(argv[i], "-server", 7) == 0) 
        {
            // Copy the server address to config.serverName
            strncpy(config.serverName, argv[i] + 7, sizeof(config.serverName) - 1);
        }
    }


    // Check that the username and server address are provided
    if (strlen(config.userId) == 0 || strlen(config.serverName) == 0) 
    {
        showUsage(argv[0]);
        return EXIT_FAILURE;
    }

    printf("[CLIENT Debug] Connecting to %s as %s...\n", config.serverName, config.userId);

    // Connect
    int sockfd = connectToServer(config.serverName, DEFAULT_PORT);

    // Send the username to the server
    sendUserId(sockfd, config.userId);

    // Run the UI and recived the thread
    initUi();

    // Launch a thread to receive messages
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receiveMessages, &sockfd);


    // Main loop: send user input to server
    char input[81] = {0};
    while (1) 
    {
        getUserInput(input, 80);
        if (strcmp(input, ">>bye<<") == 0) 
        {
            send(sockfd, input, strlen(input), 0);
            break;
        }
        send(sockfd, input, strlen(input), 0);
    }


    // Clean up UI and close socket
    destroyUi();
    close(sockfd);


    return 0;
}



// Function Name: showUsage
// Parameters:
//     const char *progname - The name of the program (argv[0]).
// Return Value: None
// Description:
//     Prints a usage message explaining the required command-line arguments.
//     This function is called when arguments are missing or invalid.

void showUsage(const char *progname) 
{
    printf("Usage: %s -user<name> -server<host>\n", progname);
}
