#include "simple-chat_functions.h"

char buff[MAX_MSG_SIZE];
char message[MAX_MSG_SIZE];
int buff_bytes = 0;

DWORD WINAPI receive_messages(LPVOID socketFileDescriptor);

int main(void) {
	
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

	HANDLE thread = CreateThread(NULL, 0, receive_messages, (LPVOID)&socketFileDescriptor, 0, NULL);
	while(1){
		printf("> ");
		scanf("%s", message);
		send(socketFileDescriptor, message, strlen(message), 0);
	}
	
	closesocket(socketFileDescriptor);
	return 0;
}

DWORD WINAPI receive_messages(LPVOID socketFileDescriptor){
	while((bytes_read = recv(socketFileDescriptor, buff, sizeof(buff), 0)) > 0){
		printf("Received: %s", buff);
	}
	return 0;
}
