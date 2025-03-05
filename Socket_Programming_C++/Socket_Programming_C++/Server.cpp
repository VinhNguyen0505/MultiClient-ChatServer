#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void handle_client(int client_socket) {
    char buffer[1024];

    // Receive username
    memset(buffer, 0, 1024);
    recv(client_socket, buffer, 1024, 0);
    std::string username = buffer;

    std::cout << username << " has joined!" << std::endl;

    while (true) {
        memset(buffer, 0, 1024);
        int bytesReceived = recv(client_socket, buffer, 1024, 0);

        if (bytesReceived <= 0) {
            std::cout << username << " disconnected." << std::endl;
            closesocket(client_socket);
            return;
        }

        std::string message = buffer;

        // Check if user wants to quit
        if (message == "/quit") {
            std::cout << username << " has left." << std::endl;
            closesocket(client_socket);
            return;
        }

        std::cout << "[" << username << "]:" << message << std::endl;

        // Send response back to client
        std::string response = "Message received!";
        send(client_socket, response.c_str(), response.size(), 0);
    }
}

int main() {
    WSADATA wsaData;
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return -1;
    }

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return -1;
    }

    // Bind socket to a port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    // Start listening
    if (listen(server_fd, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }
    std::cout << "Server listening on port " << PORT << std::endl;

    std::vector<std::thread> client_threads;

    while (true) {  // Keep accepting new clients
        std::cout << "Waiting for a new client to connect..." << std::endl;

        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed!" << std::endl;
            continue; // Try accepting again
        }

        std::cout << "Client connected!" << std::endl;

        client_threads.push_back(std::thread(handle_client, client_socket));
        client_threads.back().detach();
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
