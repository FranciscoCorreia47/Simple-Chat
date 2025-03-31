#include "simple-chat_functions.h"


int main(void) {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = initialize_Socket_IPv4();

	struct sockaddr_in serverAddress = generate_IPv4_Address("", PORT);

	bind(server, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	listen(server, 5);

	SOCKET client = accept(server, NULL, NULL);

	char message[MAX_MSG_SIZE] = { 0 };

	recv(client, message, MAX_MSG_SIZE, 0);

	wprintf(L"Received message %s", message);

	closesocket(client);
	closesocket(server);

	WSACleanup();

	return 0;
}
