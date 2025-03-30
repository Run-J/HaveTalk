#ifndef UI_H
#define UI_H

#include <ncurses.h>

void initUi();
void destroyUi();
void displayMessage(const char *msg);
void getUserInput(char *buffer, int maxLen);

#endif
