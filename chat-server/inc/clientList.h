
#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#define MAX_CLIENTS 10

void initClientList();
void addClient(int socket, const char *ip, const char *userId);
void removeClient(int socket);
int getClientCount();
void broadcastMessage(const char *fromUser, const char *fromIp, const char *message);

#endif
