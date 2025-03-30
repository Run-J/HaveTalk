#ifndef CLIENT_H
#define CLIENT_H

#define MAX_NAME_LEN 6

typedef struct 
{
    char userId[MAX_NAME_LEN];
    char serverName[64];
} ClientConfig;

void showUsage(const char *progname);


#endif
