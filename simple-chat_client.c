#include "simple-chat_functions.h"

int main(void) {

	char message[MAX_MSG_SIZE];

	char* ip = "127.0.0.1";
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);

	struct sockaddr_in *serverAddress = generate_IPv4_Address(ip, PORT);

	int socketFileDescriptor = initialize_Socket_IPv4();

	int connection = connect(socketFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	if (connection == 0)
		printf("Connection was successful!\n");
	else
		printf("Connection error!\n");
	
	printf("> ");
	scanf("%s", message);
	send(socketFileDescriptor, message, strlen(message), 0);
	recv(socketFileDescriptor, message, MAX_MSG_SIZE, 0);

	printf("Response: %s\n", message);

	closesocket(socketFileDescriptor);

	return 0;
}
