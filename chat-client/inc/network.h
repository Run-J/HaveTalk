#ifndef NETWORK_H
#define NETWORK_H

#include "client.h"

int connectToServer(const char *serverName, int port);
void sendUserId(int sockfd, const char *userId);

#endif
