#include "../inc/clientHandler.h"
#include "../inc/clientList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <arpa/inet.h>




// Function Name: handleClient
// Parameters:
//     void *arg - A pointer to dynamically allocated memory containing the client's socket descriptor.
// Return Value:
//     void* - Always returns NULL; designed to be used as a thread function.
// Description:
//     This function runs in a separate thread for each connected client. It first receives the client's
//     username, adds the client to a global client list, and enters a loop to receive chat messages.
//     Messages are broadcast to all connected clients using broadcastMessage().
//     If the client sends a disconnect command (e.g., "bye") or the connection drops,
//     the client is removed from the list, and the connection is closed.
//     Memory passed in via arg is freed at the beginning of the function.
//     This function is thread-safe and works with other components via mutex-protected operations.

void *handleClient(void *arg) 
{
    // Fetch the socket descriptor and free parameter memory
    int clientSocket = *((int *)arg);
    free(arg);  // Parameters are allocated via malloc and are freed here.

    char userId[6] = {0};
    char buffer[MAX_MESSAGE_LEN + 1] = { 0 };  // Buffer for receiving client messages


    // Get client IP address information
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    getpeername(clientSocket, (struct sockaddr *)&addr, &addrLen);

    char ipStr[INET_ADDRSTRLEN] = { 0 };  // IP address in string form (max 15 characters + '\0')
    inet_ntop(AF_INET, &addr.sin_addr, ipStr, sizeof(ipStr));  // Convert to a readable IP string


    // Step 1: Receive Username
    if (recv(clientSocket, userId, sizeof(userId) - 1, 0) <= 0)  // without '\0'
    {
        // If reception fails, close the connection and exit the thread
        close(clientSocket);
        return NULL;
    }

    printf("[SERVER Debug] Client joined: %s (%s)\n", userId, ipStr);



    // Step 2: Add the client to the global client list (for broadcasting, etc.)
    addClient(clientSocket, ipStr, userId);


    // Step 3: Receive Message Loop
    while (1) 
    {
        memset(buffer, 0, sizeof(buffer)); // Empty the buffer
        int bytesReceived = recv(clientSocket, buffer, MAX_MESSAGE_LEN, 0);

        if (bytesReceived <= 0) 
        {
            // Client disconnects or an error occurs
            break;
        }

        buffer[bytesReceived] = '\0'; // Add string terminator to prevent dirty data

        // Special command (bye): client-initiated disconnection
        if (strcmp(buffer, ">>bye<<") == 0)
        {
            printf("[SERVER Debug] %s (%s) disconnected.\n", userId, ipStr);
            break;
        }

        // Step 4: Broadcast message
        broadcastMessage(userId, ipStr, buffer);
    }

    // Step 5: Remove the client
    removeClient(clientSocket);
    close(clientSocket);

    return NULL; // End of thread
}