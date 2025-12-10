# 💬 Text Chat Application — Client–Server Messaging System

This project implements a fully functional **client–server text chat system** built using TCP sockets and the `select()` system call.  
It supports multi-client communication, message relaying through a central server, user-level blocking, message buffering, and server-side statistics tracking.

The application runs in two modes—**server mode** and **client mode**—with behavior determined by command-line arguments.  
All components use low-level socket APIs to provide efficient, event-driven communication without multithreading.

---

## 🚀 Features Overview

### 🖥️ Server Capabilities
- Accepts multiple client connections  
- Tracks and manages logged-in clients  
- Relays messages between clients  
- Buffers messages for offline clients  
- Maintains usage statistics:
  - Number of messages sent  
  - Number of messages received  
  - Online/offline status  
- Provides administrative commands:
  - `IP`, `PORT`, `LIST`  
  - `STATISTICS`  
  - `BLOCKED <client-ip>`

---

### 👤 Client Capabilities
- Connects to server for text-based communication  
- Sends direct messages (`SEND`)  
- Sends broadcast messages (`BROADCAST`)  
- Maintains a local block list  
- Receives server-delivered and buffered messages  
- Supports the following commands:
  - `IP`, `PORT`, `LIST`  
  - `LOGIN <server-ip> <server-port>`  
  - `REFRESH`  
  - `SEND <client-ip> <msg>`  
  - `BROADCAST <msg>`  
  - `BLOCK <client-ip>` / `UNBLOCK <client-ip>`  
  - `LOGOUT`  
  - `EXIT`

---

## 📡 Network Model

- All communication uses **TCP sockets**.  
- Input multiplexing is handled using **`select()`**, enabling simultaneous monitoring of keyboard input and socket events.  
- The server acts as a **central relay** — clients do not communicate directly with each other.  
- Each connected client is identified by its IP address and a server-assigned port.

---

## 🧩 Command Behavior

### Common Commands (Server & Client)

| Command | Description |
|--------|-------------|
| `IP` | Displays the external IP address used for communication. |
| `PORT` | Displays the port the application is running on. |
| `LIST` | Shows the list of currently logged-in clients (sorted by port). |

---

## 🖥️ Server-Only Commands

### `STATISTICS`
Displays, for each known client:
- Hostname  
- Number of messages sent  
- Number of messages received  
- Online/offline status  

Sorted by port.

### `BLOCKED <client-ip>`
Shows all clients that have been blocked **by** the specified client.

---

## 👤 Client-Only Commands

### `LOGIN <server-ip> <server-port>`
Registers the client with the server and retrieves:
- Current list of logged-in clients  
- Any buffered messages  

### `REFRESH`
Requests an updated list of connected clients.

### `SEND <client-ip> <msg>`
Sends a unicast message to a specific client.

### `BROADCAST <msg>`
Sends a message to all connected clients.

### `BLOCK <client-ip>` / `UNBLOCK <client-ip>`
Updates the client's block list.

### `LOGOUT`
Disconnects the client from the server.

### `EXIT`
Closes the client program and removes all server-side state.

---

## 📂 Project Structure

```
project/
├── src/
│   ├── server.cpp
│   ├── client.cpp
│   ├── logger.cpp
│   ├── utility.cpp
│   └── ...
├── include/
│   ├── logger.hpp
│   ├── commands.hpp
│   └── ...
├── Makefile
└── README.md


```

---

## 🔧 Technologies Used

- **C++**
- **TCP/IP socket programming**
- **Linux environment**


---


