#include "simple-chat_functions.h"

char buff[MAX_MSG_SIZE];
char message[MAX_MSG_SIZE];
int buff_bytes = 0;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void* receive_messages(void* serverSocket);

int main(void) {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	pthread_t thread1;
	char* serverIp;

	printf("Insert the server's IP?");
	scanf("%s", serverIp);

	SOCKET server = initialize_Socket_IPv4();

	struct sockaddr_in server_address = generate_IPv4_Address(serverIp, PORT);

	/*short int checkPort = checkPort5000();
	if (checkPort) {
		Sleep(2000);
		return 1;
	}*/

	int connectionResult = connect(server, (struct sockaddr*)&server_address, sizeof(server_address));
	switch (connectionResult) {
	case SOCKET_ERROR:
		wprintf(L"Failed to Connect to the server %ld\n", WSAGetLastError());
		closesocket(server);
		WSACleanup();
		return 1;
	default:
		wprintf(L"Connected\n");
		break;
	}

	pthread_mutex_init(&print_mutex, NULL);
	pthread_create(&thread1, NULL, *receive_messages, (void *)&server);

	char message[MAX_MSG_SIZE] = { 0 };
	while(1) {

		memset(message, 0, sizeof(message));
		pthread_mutex_lock(&print_mutex);
		printf("Write your message: ");
		scanf("%s", message);
		pthread_mutex_unlock(&print_mutex);
		encrypt(message);
		send(server, message, MAX_MSG_SIZE, 0);

	}

	printf("Bye!\n");
	Sleep(2000);

	closesocket(server);
	pthread_join(thread1, NULL);
	WSACleanup();

	return 0;
}

void* receive_messages(void* serverSocket) {
	buff[buff_bytes] = '\0';
	SOCKET* server = (SOCKET*)serverSocket;
	while ((buff_bytes = recv(*server, buff, sizeof(buff), 0)) > 0) {
		pthread_mutex_lock(&print_mutex);
		encrypt(buff);
		wprintf(L"Received: %s\n", buff);
		fflush(stdout);
		pthread_mutex_unlock(&print_mutex);
	}
	return NULL;
}
