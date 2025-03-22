# MultiClient Chat Server
A real-time chat application built using C++ sockets that allows multiple clients to connect and communicate through a TCP server.

# Features:
- Accept multiple client connections using multithreading
- Real-time message broadcasting to all connected clients
- Clean and simple client-server architecture
- Includes both **server and client** code

# Technologies Used:
- C++
- Sockets (Windows API / POSIX)
- Multi-threading

# How to Run the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/VinhNguyen0505/multiclient_chat_server.git
   cd multiclient_chat_server
   ```

2. Compile the server and client code (Windows example with `g++`):

   **Server:**
   ```bash
   g++ server.cpp -o server -lws2_32
   ```

   **Client:**
   ```bash
   g++ client.cpp -o client -lws2_32
   ```

3. Start the server:

   ```bash
   ./server
   ```

4. Start one or more clients in separate terminals:

   ```bash
   ./client
   ```
