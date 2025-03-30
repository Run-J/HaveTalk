#include "../inc/ui.h"

#include <string.h>

#define MAX_LINES 10  // Max number of lines shown in the chat window


// Global UI components
static WINDOW *chatWin;        // Window for displaying chat messages
static WINDOW *inputWin;       // Window for user input
static char messageHistory[MAX_LINES][128];    // Circular message buffer
static int currentLine = 0;    // Index of current message line


// Function Name: initUi
// Parameters: None
// Return Value: None
// Description:
//     Initializes ncurses and sets up two windows:
//     - chatWin for displaying past messages
//     - inputWin for receiving user input
void initUi() 
{
    initscr();  // Start ncurses mode
    cbreak();   // Disable line buffering
    // noecho();   // Don't echo input characters
    refresh();  // Refresh default screen

    int height = 12;  // Height of chat window
    int width = COLS;  // Full terminal widths


    // Create windows: chat on top, input at bottom
    chatWin = newwin(height, width, 0, 0);
    inputWin = newwin(3, width, height, 0);


    scrollok(chatWin, TRUE);  // Enable scrolling in chat window
    box(chatWin, 0, 0);   // Draw border
    box(inputWin, 0, 0);  // Draw border

    wrefresh(chatWin);
    wrefresh(inputWin);
}



// Function Name: destroyUi
// Parameters: None
// Return Value: None
// Description:
//     Cleans up ncurses windows and exits ncurses mode.
void destroyUi() 
{
    delwin(chatWin);
    delwin(inputWin);
    endwin();  // End ncurses mode
}



// Function Name: displayMessage
// Parameters:
//     const char *msg - The message to display
// Return Value: None
// Description:
//     Adds a new message to the chat window and redraws the UI.
//     Maintains a rolling message buffer of the last MAX_LINES messages.
void displayMessage(const char *msg)
{
    // Shift history up if at max capacity
    if (currentLine == MAX_LINES) 
    {
        for (int i = 1; i < MAX_LINES; i++)
        {
            strcpy(messageHistory[i - 1], messageHistory[i]);
        }
        currentLine--;
    }

    // Add new message to history
    strncpy(messageHistory[currentLine++], msg, sizeof(messageHistory[0]));


    // Clear chat window and redraw all messages
    werase(chatWin);
    box(chatWin, 0, 0);
    for (int i = 0; i < currentLine; i++) 
    {
        mvwprintw(chatWin, i + 1, 1, "%s", messageHistory[i]);   // Print each message
    }

    wrefresh(chatWin);
}



// Function Name: getUserInput
// Parameters:
//     char *buffer    - Destination buffer to store user input
//     int max_len     - Maximum input length
// Return Value: None
// Description:
//     Displays the input window, waits for the user to type a message,
//     and stores it in the provided buffer.
void getUserInput(char *buffer, int maxLen) 
{
    werase(inputWin);  // Clear input window
    box(inputWin, 0, 0);  // Redraw border
    mvwgetnstr(inputWin, 1, 1, buffer, maxLen);  // Get input from user
    wrefresh(inputWin);
}
