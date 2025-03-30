#include "../inc/clientList.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>


typedef struct 
{
    int socket;         // Client socket file descriptor
    char ip[INET_ADDRSTRLEN];       // Client IP address (string)
    char userId[6];        // User name (up to 5 characters + '\0')
    int active;     // Activity status (1 for online)
} Client;



// Global client arrays and mutex locks (thread-safe)
static Client clients[MAX_CLIENTS];     // up to MAX_CLIENTS clients
static pthread_mutex_t clientMutex = PTHREAD_MUTEX_INITIALIZER;



// Function Name: initClientList
// Parameters: None
// Return Value: None
// Description:
//     Initializes the global client array by setting all entries to zero.
//     This should be called once at the start of the server to prepare
//     the client list for future connections.

void initClientList()
{
    memset(clients, 0, sizeof(clients));  // Zero out the entire array of structures
}



// Function Name: addClient
// Parameters:
//     int socket         - The client socket descriptor.
//     const char *ip     - The client's IP address string.
//     const char *userId - The client's username.
// Return Value: None
// Description:
//     Adds a new client to the global client list by finding the first
//     available (inactive) slot and storing the socket, IP, and user ID.
//     This function is thread-safe using a mutex lock.

void addClient(int socket, const char *ip, const char *userId) 
{
    pthread_mutex_lock(&clientMutex);
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (!clients[i].active)   // Find an empty space
        {
            clients[i].socket = socket;
            strncpy(clients[i].ip, ip, sizeof(clients[i].ip));
            strncpy(clients[i].userId, userId, sizeof(clients[i].userId));
            clients[i].active = 1;  // Mark it active
            break;
        }
    }
    pthread_mutex_unlock(&clientMutex);
}




// Function Name: removeClient
// Parameters:
//     int socket - The client socket descriptor to be removed.
// Return Value: None
// Description:
//     Marks a client as inactive in the global client list by
//     matching the provided socket descriptor.
//     Thread-safe with mutex protection.

void removeClient(int socket) 
{
    pthread_mutex_lock(&clientMutex);
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (clients[i].active && clients[i].socket == socket) 
        {
            clients[i].active = 0;  // Marked as inactive (cleared)
            break;
        }
    }
    pthread_mutex_unlock(&clientMutex);
}



// Function Name: getClientCount
// Parameters: None
// Return Value:
//     int - The number of currently active (connected) clients.
// Description:
//     Counts how many clients in the global client list are marked
//     as active (i.e., currently connected). Thread-safe.

int getClientCount() 
{
    int counter = 0;
    pthread_mutex_lock(&clientMutex);
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (clients[i].active)
        {
            counter++;
        }
    }
    pthread_mutex_unlock(&clientMutex);
    return counter;
}




// Function Name: broadcastMessage
// Parameters:
//     const char *fromUser - The sender's username.
//     const char *fromIp   - The sender's IP address.
//     const char *message  - The message to broadcast.
// Return Value: None
// Description:
//     Sends a formatted message to all active clients in the list.
//     The message includes sender IP, username, timestamp, and is
//     split into 40-character chunks if too long.
//     This function is thread-safe and ensures mutual exclusion
//     during iteration and send operations.

void broadcastMessage(const char *fromUser, const char *fromIp, const char *message) 
{
    char formatted[128] = { 0 };

    // Generate the current time string (HH:MM:SS)
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timeStr[10] = {0};
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", tm_info);

    // Split long messages into up to two 40-character paragraphs
    size_t len = strlen(message);
    size_t offset = 0;

    pthread_mutex_lock(&clientMutex);

    while (offset < len) 
    {
        char chunk[41] = {0}; // 40 + \0; Up to 40 characters per paragraph
        size_t chunkLength = len - offset > 40 ? 40 : len - offset;
        strncpy(chunk, message + offset, chunkLength);
        offset = offset + chunkLength;

        // Format message with IP, username, message content, timestamp
        snprintf(formatted, sizeof(formatted), "%-15s [%5s] << %-40s (%s)", fromIp, fromUser, chunk, timeStr);

        // Send to all active clients
        for (int i = 0; i < MAX_CLIENTS; i++) 
        {
            if (clients[i].active) 
            {
                send(clients[i].socket, formatted, strlen(formatted), 0);
            }
        }
    }

    pthread_mutex_unlock(&clientMutex);
}
