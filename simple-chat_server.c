#include "simple-chat_functions.h"

void* receive_messages(void* clientSocket);

char buff[MAX_MSG_SIZE];
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
int bytes_read = 0;


int main(void) {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	pthread_t thread1;

	SOCKET server = initialize_Socket_IPv4();

	struct sockaddr_in serverAddress = generate_IPv4_Address("", PORT);

	bind(server, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	int checkIP = getWirelessIP();
	if (checkIP) {
		Sleep(2000);
		return 1;
	}

	listen(server, 5);

	SOCKET client = accept(server, NULL, NULL);

	pthread_mutex_init(&print_mutex, NULL);
	pthread_create(&thread1, NULL, receive_messages, (void*) &client);

	pthread_join(&thread1, NULL);

	closesocket(client);
	closesocket(server);

	WSACleanup();

	return 0;
}

void* receive_messages(void* clientSocket) {
	SOCKET* client = (SOCKET*)clientSocket;
	buff[bytes_read] = '\0';
	while ((bytes_read = recv(*client, buff, MAX_MSG_SIZE, 0)) > 0) {
		pthread_mutex_lock(&print_mutex);
		printf("Message: %s\n", buff);
		fflush(stdout);
		pthread_mutex_unlock(&print_mutex);

		send(*client, buff, bytes_read, 0);
	}
	return NULL;
}
