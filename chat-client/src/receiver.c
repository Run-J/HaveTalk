#include "../inc/ui.h"


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>


void *receiveMessages(void *arg) 
{
    int sockfd = *(int *)arg;
    char buffer[128] = {0};

    while (1) 
    {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0)
        {
            break;
        }
        buffer[n] = '\0';
        displayMessage(buffer);
    }

    return NULL;
}