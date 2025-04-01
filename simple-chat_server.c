#include "simple-chat_functions.h"

/*

  The function send_messages should receive the clientFD as a parameter, for internal use

*/


DWORD WINAPI send_messages(LPVOID serverFD);

char buff[MAX_MSG_SIZE] = { 0 };
char message[MAX_MSG_SIZE] = { 0 };
int bytes_read = 0;

int main(void) {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = initialize_Socket_IPv4();
	struct sockaddr_in serverAddress = generate_IPv4_Address("", PORT);

	bind(server, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
  listen(server, 5);
  
	SOCKET client = accept(server, NULL, NULL);

		while((bytes_read = recv(client, buff, MAX_MSG_SIZE, 0)) > 0){
		printf("Message: %s\n", buff);
	}
	HANDLE thread = CreateThread(NULL, send_messages, (LPVOID)&server, 0, NULL);

	closesocket(client);
	closesocket(server);

	WSACleanup();
		
	return 0;
}

DWORD WINAPI send_messages(LPVOID serverFD){
	send(clientFD, buff, bytes_read, 0);
	return 0;
}
