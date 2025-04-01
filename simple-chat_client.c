#include "simple-chat_functions.h"

DWORD WINAPI receive_messages(LPVOID socketFileDescriptor);

char buff[MAX_MSG_SIZE];
char message[MAX_MSG_SIZE];
int buff_bytes = 0;

int main(void) {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET client = initialize_Socket_IPv4();

	struct sockaddr_in client_address = generate_IPv4_Address("127.0.0.1", PORT);

	int connectionResult = connect(client, (struct sockaddr*)&client_address, sizeof(client_address));
	switch (connectionResult) {
		case SOCKET_ERROR:
			wprintf(L"Failed to Connect to the server %ld\n", WSAGetLastError());
			closesocket(client);
			WSACleanup();
			return 1;
		default:
			wprintf(L"Connected\n");
			break;
  }

	HANDLE thread = CreateThread(NULL, 0, receive_messages, (LPVOID)&socketFileDescriptor, 0, NULL);
	while(1){
		printf("> ");
		scanf("%s", message);
		send(socketFileDescriptor, message, strlen(message), 0);
	}

	Sleep(2000);

	closesocket(client);
	WSACleanup();
  
  return 0;
}

DWORD WINAPI receive_messages(LPVOID socketFileDescriptor){
	while((bytes_read = recv(socketFileDescriptor, buff, sizeof(buff), 0)) > 0){
		printf("Received: %s", buff);
	}
	return 0;
}


