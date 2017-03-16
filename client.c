#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
#include <stdlib.h>
#define MAX_BUFFER_SIZE 100
int main(int argc, char const *argv[]) {

	if(argc != 2){
		fprintf(stderr, "Usage: client <serverIP>\n");
		exit(1);
	}

	WSADATA wsaData;
	int result;
	result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0) {
	    printf("WSAStartup failed: %d\n", result);
	    exit(1);
	}


	// Create socket
	int sockId = socket(PF_INET, SOCK_STREAM, 0);
	if(sockId == -1) { // check if successfull
		printf("Failed\n");
		exit(1);
	}
	else {
		printf("Opened stream\n");
	}


	// Create network address data structure
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(3030); // big endian / little endian
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);


	// Connect 
	int status = connect(sockId, (struct sockaddr *) &serverAddress, sizeof(serverAddress)); // connect is a blocking operation?
	if(status == 0) {
		printf("Successfully connected to server\n");
	}
	else {
		printf("Could not successfully connect\n");
		exit(1);
	}


	char sendString[MAX_BUFFER_SIZE];
	char receiveString[MAX_BUFFER_SIZE];
	int length, msg_size;
	while(fgets(sendString, MAX_BUFFER_SIZE, stdin) != NULL) {
		length = strlen(sendString);
		sendString[length-1] = 0; // get rid of newline
		length = strlen(sendString);
		if(send(sockId, sendString, length+1, 0) != length+1) {
			printf("send() failed\n");
			exit(1);
		}
		msg_size = recv(sockId, receiveString, MAX_BUFFER_SIZE, 0);
		if(msg_size < 0) {
			printf("Receive failed()\n");
			exit(1);
		}
		else {
			printf("Response received: %s\n", receiveString);
			memset(&receiveString[0], 0, sizeof(receiveString));
		}

	}
	// close(sockId); // Unix
	closesocket(sockId); // Windows
}
