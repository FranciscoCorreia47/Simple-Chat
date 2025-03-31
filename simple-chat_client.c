#include "simple-chat_functions.h"

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

	char message[MAX_MSG_SIZE] = { 0 };
	printf("Write your message: ");
	scanf("%s", message);

	send(client, message, MAX_MSG_SIZE, 0);

	recv(client, message, MAX_MSG_SIZE, 0);

	wprintf(L"%s", message);

	Sleep(2000);

	closesocket(client);
	WSACleanup();

	return 0;
}
