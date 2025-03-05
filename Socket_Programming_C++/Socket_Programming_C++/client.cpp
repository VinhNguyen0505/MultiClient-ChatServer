#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
	WSADATA wsaData;
	int sock;
	struct sockaddr_in server_address;

	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed!" << std::endl;
		return -1;
	}

	// Create the socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Socket creation failed!" << std::endl;
		WSACleanup();
		return -1;
	}
	// Connect to server
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

	if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
		std::cerr << "Connection failed!" << std::endl;
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	std::cout << "Connected to server!" << std::endl;

	// Ask for username
	std::cout << "Enter your username: ";
	std::string username;
	std::getline(std::cin, username);

	// Send username to server
	send(sock, username.c_str(), username.size(), 0);

	char buffer[1024] = { 0 };
	std::string message;

	while (true) {
		std::cout << "Enter message: ";
		std::getline(std::cin, message);

		if (message == "/quit") {
			std::cout << "Disconnecting from server..." << std::endl;
			send(sock, message.c_str(), message.size(), 0);
			break;
		}

		send(sock, message.c_str(), message.size(), 0);

		int bytesReceived = recv(sock, buffer, 1024, 0);
		if (bytesReceived <= 0) {
			std::cout << "Server disconnected." << std::endl;
			break;
		}
		std::cout << "Server: " << buffer << std::endl;
	}

	closesocket(sock);
	WSACleanup();
	return 0;

}