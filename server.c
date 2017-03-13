#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
#include <stdlib.h>

void handler(int);

int main(void) {

	WSADATA wsaData;
	int result;
	result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0) {
	    printf("WSAStartup failed: %d\n", result);
	    exit(1);
	}
	// Opens the stream
	int socketId = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketId == -1) { // check if successfull
		printf("Failed\n");
		exit(1);
	}
	else {
		printf("Opened stream\n");
	}

	// Create a new address data structure
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(3030);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	// Assign an address to socket
	int status = bind(socketId, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
	if(status == -1) {
		printf("Failed to bind address\n");
		exit(1);
	}

	// Binding is skipped:
	// - destination is determined during connection setup
	// - we don't need to know which port we're sending from

	// listen(sockId, queueLimit)
	// queueLimit: # of active participants that can listen
	status = listen(socketId, 10);
	if(status == 0) {
		printf("Listening on port 3030\n");
	}
	else {
		printf("Could not open connection\n");
		exit(1);
	}

	struct sockaddr_in clientAddr;
	int clntLen;
	int clntSock;
	while(1) {
		clntLen = sizeof(clientAddr);
		// Establish a connection
		if((clntSock = accept(socketId, (struct sockaddr *) &clientAddr, &clntLen)) < 0) {
			printf("Could not accept connection\n");
			exit(1);
		}

		printf("Handling client %s\n", inet_ntoa(clientAddr.sin_addr));
		handler(clntSock);
		printf("Client %s disconnected\n", inet_ntoa(clientAddr.sin_addr));
		// Commmunicate
		// Close the connection
	}


	// Closes the streams AND frees the port
	// status = close(socketId); // Unix
	status = closesocket(socketId); // Windows
	if(status == 0) {
		printf("Closed stream\n");
	}
}
#define RECEIVE_BUFFER_SIZE 55
void handler(int clientSocket) {
	char buffer[RECEIVE_BUFFER_SIZE]; // maxlength buffer
	int msg_size;
	if((msg_size = recv(clientSocket, buffer, RECEIVE_BUFFER_SIZE, 0)) < 0) {
		printf("Error receving message\n");
		exit(1);
	}
	while(msg_size > 0) { // 0 indicates end of transmission
		if(send(clientSocket, buffer, msg_size, 0) != msg_size) {
			printf("send() failed\n");
			exit(1);
		}
		if((msg_size = recv(clientSocket, buffer, RECEIVE_BUFFER_SIZE, 0)) < 0) {
			printf("Receive failed()\n");
			exit(1);
		}
	}
	// close(clientSocket); // Unix
	closesocket(clientSocket); // Windows
}

