#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

// Maximum length per message (up to 80 characters at a time)
#define MAX_MESSAGE_LEN 80

void *handleClient(void *arg);

#endif