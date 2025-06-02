#include "simple-chat_functions.h"

char			buff[MAX_MSG_SIZE];
int				buff_bytes  = 0;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void* receive_messages(void* serverSocket);

int main(void) {

	// Starting up the WinSock API
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	pthread_t thread1;
	char serverIp[INET_ADDRSTRLEN];

	printf("Insert the server's IP?\n>");
	scanf("%s", serverIp);

	// Initializing the Server Socket
	SOCKET server = initialize_Socket_IPv4();

	// Creating the IPv4 address structure using the IP that was parsed
	struct sockaddr_in server_address = generate_IPv4_Address(serverIp, PORT);

	// Attempting connection to the server
	int connectionResult = connect(server, (struct sockaddr*)&server_address, sizeof(server_address));
	switch (connectionResult) {
	case SOCKET_ERROR:
		wprintf(L"Failed to Connect to the server %ld\n", WSAGetLastError());
		closesocket(server);
		WSACleanup();
		return 1;
	default:
		printf("Connected to server on %s\n", serverIp);
		break;
	}

	// Initializing both the thread for receiving messages and the mutex to handle race conditions
	pthread_mutex_init(&print_mutex, NULL);
	pthread_create(&thread1, NULL, receive_messages, (void *)&server);

	// Code to get message from user, and sending it to server
	char message[MAX_MSG_SIZE] = { 0 };
	while(1) {

		memset(message, 0, sizeof(message));
		
		// Lock here to avoid stdin flood caused by the thread receiving and printing the message
		pthread_mutex_lock(&print_mutex);

		flush_input();

		printf("> ");
		
		if (fgets(message, MAX_MSG_SIZE, stdin)) {
			// Strip newline if present
			size_t len = strlen(message);
			if (len > 0 && message[len - 1] == '\n') {
				message[len - 1] = '\0';
			}
		}

		fflush(stdout);

		pthread_mutex_unlock(&print_mutex);
		
		// Check for exit, send message to server (if exiting, no encription, else, encrypt)
		if (strcmp(message, "/exit") == 0) {
			send(server, message, MAX_MSG_SIZE, 0);
			break;
		}
		else {
			encrypt(message);
			send(server, message, sizeof(message), 0);
		}
		Sleep(500);
	}

	printf("Bye!\n");
	Sleep(2000);

	pthread_join(thread1, NULL);
	closesocket(server);
	WSACleanup();

	return 0;
}

// The function to receive the messages from the server, used on the thread
void* receive_messages(void* serverSocket) {
	
	// Re-typecasting the server SOCKET back to it's original type
	SOCKET* server = (SOCKET*)serverSocket;
	
	while (1) {
		// Cleaning up the buffer before receiving new message
		memset(buff, 0, sizeof(buff));
		buff_bytes = recv(*server, buff, MAX_MSG_SIZE, 0);

		if (buff_bytes == 0)
			break;

		buff[buff_bytes] = '\0';

		// Lock here to avoid interrupting message typing by user
		pthread_mutex_lock(&print_mutex);

		// Decrypt the received message for printing
		encrypt(buff);
		printf("Received: %s\n", buff);
		fflush(stdout);
		pthread_mutex_unlock(&print_mutex);
	}
	return NULL;
}
