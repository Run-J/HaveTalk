# 💬 Can We Talk — Terminal Chat System in C

A lightweight multi-threaded chat system built in C using TCP sockets and `ncurses`, featuring real-time communication in the terminal with modular client-server architecture.

<div align="center">
  <img src="https://github.com/user-attachments/assets/480fcc64-d638-4077-b51b-8c5d2e112cde" alt="talkDemo">
</div>
---

## 📂 Project Structure

```
SPA04/ 
├── chat-server/                               # Multi-threaded TCP server 
│ ├── src/                                     # Server source code 
│ ├── inc/                                     # Server headers 
│ ├── bin/                                     # Compiled server binary 
├── chat-client/                               # Terminal-based client using ncurses 
│ ├── src/                                     # Client source code 
│ ├── inc/                                     # Client headers 
│ ├── bin/                                     # Compiled client binary 
├── common/                                    # Shared output directory (bin/) 
│ ├── bin/                                     # Unified executable storage 
├── Makefile                                   # Root Makefile for building both server and client
```

---

## 🧠 Key Concepts Learned

### 🔌 Network Programming with Sockets
- Used **TCP socket APIs**: `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `send()`, `recv()`
- Applied `htons()` and `inet_ntop()` for IP/port handling
- Implemented **connection limit** using server-side checks (`MAX_CLIENTS`)

### 🧵 Multi-threading with POSIX `pthread`
- Used `pthread_create()` to handle each client on a separate thread
- Applied `pthread_mutex_t` to synchronize access to shared resources (client list)
- Used detached threads for automatic cleanup with `pthread_detach()`

### 💻 Terminal UI with `ncurses`
- Built a split-window interface:
  - Upper window for chat history
  - Lower input window with `getUserInput()`
- Implemented dynamic message rendering and scroll-like effect
- Used `box()`, `mvwprintw()`, `mvwgetnstr()`, `echo()/noecho()` for clean UX

### 🧱 Modular Design
- Clear separation between:
  - `connection.c`: Accepting new connections
  - `clientList.c`: Managing connected users & broadcasting
  - `clientHandler.c`: Handling per-client interaction
  - `receiver.c` (client): Thread to handle server responses
  - `ui.c`: Clean interface via `ncurses`

---

## 🚀 How to Run

```bash
# From the root directory
make                            # Build server and client
./common/bin/chat-server        # Start server
./common/bin/chat-client -user<name> -server<ip>  # Start client
```

---

## ❗ Potential Difficulties
- Thread safety: Managing shared client list with mutexes
- Socket lifecycles: Proper closing, memory allocation, and freeing per-thread
