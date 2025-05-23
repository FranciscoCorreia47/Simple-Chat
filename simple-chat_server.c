#include "simple-chat_functions.h"

void* forward_messages(void* clientSocket);
void* receive_messages(void* clientSocket);

char 			buff[5][MAX_MSG_SIZE];
int 			bytes_read = 0;
pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
	
	// Starting up the WinSock API
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	
	pthread_t thread1, thread2;

	// Initializing the Server Socket
	SOCKET server = initialize_Socket_IPv4();

	// Generating the IPv4 address for use with the server
	// The IP being blank tells the function to use INADDR_ANY
	struct sockaddr_in serverAddress = generate_IPv4_Address("", PORT);

	// Binds the server's Socket and Address in the operating system
	bind(server, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	// Get the server's Wireless IP to let the clients know where to connect
	int checkIP = getWirelessIP();
	if (checkIP) {
		Sleep(2000);
		return 1;
	}

	// Start listening for new connections
	listen(server, 5);

	// Initialize sockets as -1 to avoid using memory trash
	SOCKET client[5] = {SOCKET_ERROR, SOCKET_ERROR, SOCKET_ERROR, SOCKET_ERROR};

	// Creating the threads to receive and send messages while receiving new connections
	pthread_create(&thread1, NULL, forward_messages, (void*)&client);
	pthread_create(&thread2, NULL, receive_messages, (void*)&client);

	// Accept incoming client connections
	int count = 0;
	while (count < 5) {
		for (int i = 0; i < 5; i++) {
			if (client[i] != SOCKET_ERROR)
				continue;
			else {
				// Lock to avoid trying to receive/send before even having clients connected
				pthread_mutex_lock(&accept_mutex);
				client[i] = accept(server, NULL, NULL);
				pthread_mutex_unlock(&accept_mutex);
				printf("Client %d Connected\n", i);
				count++;
			}
		}
	}


	// Joining the thread so that the program waits until there is no traffic to close
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	// Closing all sockets
	for (int i = 0; i < 5; i++)
		closesocket(client[i]);
	closesocket(server);

	// WinSock API function that cleans the socket usage and other resources, to avoid safety issues
	WSACleanup();

	return 0;

}

// This function receives the client socket parsed as a void*, and then typecasted back to a SOCKET*
void* receive_messages(void* clientSocket) {
	
	SOCKET* client = (SOCKET*)clientSocket;

	// Receives messages until no message was received
	while (1) {
		for (int i = 0; i < 5; i++) {
			// Sleeping here to avoid trying to send before having clients connected
			Sleep(500);
			// Cleaning the buffer to avoid memory trash
			memset(buff[i], 0, sizeof(buff));

			pthread_mutex_lock(&accept_mutex);
			// Ignore unused clients
			if (client[i] == SOCKET_ERROR)
				continue;
			
			bytes_read = recv(client[i], buff[i], MAX_MSG_SIZE, 0);
			
			// Check for disconnecting client
			if (strcmp(buff[i], "/exit") == 0) {
				client[i] = SOCKET_ERROR;
				printf("Client %d Disconnected\n", i);
				pthread_mutex_unlock(&accept_mutex);
				break;
			}
			printf("Received %d bytes from Client %d\n", bytes_read, i);
			pthread_mutex_unlock(&accept_mutex);
		}
	}
	return NULL;
}

// Sends a message to all the clients
void* forward_messages(void* clientSocket) {
	
	// Making a pointer to the entire array and typecasting it back
	SOCKET* client = (SOCKET*)clientSocket;
	
	while (1) {
		for (int i = 0; i < 5; i++) {
			// Sleeping here to avoid trying to send before accepting a client/receiving a message 
			Sleep(550);
			// Cleaning the buffer to avoid memory trash
			memset(buff[i], 0, sizeof(buff));

			// Locking here to avoid receiving at the same position while sending
			pthread_mutex_lock(&accept_mutex);
			// Not send to unused clients
			if (client[i] == SOCKET_ERROR)
				continue;

			if (bytes_read > 0) {
				buff[i][bytes_read] = '\0';
				send(client[i], buff[i], bytes_read, 0);
			}
			pthread_mutex_unlock(&accept_mutex);
		}
	}

	return NULL;
}
