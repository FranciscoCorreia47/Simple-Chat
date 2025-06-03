#include "simple-chat_functions.h"

void forward_messages(Client* client);
void* receive_messages(void* clientStruct);

char 			buff[5][MAX_MSG_SIZE];
int 			bytes_read = 0;
pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;

Client Clients[5];

int main(void) {

	// Starting up the WinSock API
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	pthread_t thread1[5];

	// Initializing the Server Socket
	SOCKET server = initialize_Socket_IPv4();

	// Generating the IPv4 address for use with the server
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

	// Accept incoming client connections
	int count = 0;
	while (count < 5) {
		for (int i = 0; i < 5; i++) {
			Clients[i].socket = SOCKET_ERROR;
			Clients[i].buffer = i;
			if (Clients[i].socket != SOCKET_ERROR)
				continue;
			else {
				// Lock to avoid trying to receive/send before even having clients connected
				pthread_mutex_lock(&accept_mutex);
				Clients[i].socket = accept(server, NULL, NULL);
				pthread_create(&thread1[i], NULL, receive_messages, (void*)&Clients[i]);
				pthread_mutex_unlock(&accept_mutex);
				if (Clients[i].socket == SOCKET_ERROR) break;
				printf("Client %d Connected\n", i+1);
				count++;
			}
		}
	}


	// Joining the thread so that the program waits until there is no traffic to close
	for (int i = 0; i < 5; i++)
		pthread_join(thread1[i], NULL);

	// Closing all sockets
	for (int i = 0; i < 5; i++)
		closesocket(Clients[i].socket);
	closesocket(server);

	// WinSock API function that cleans the socket usage and other resources, to avoid safety issues
	WSACleanup();

	return 0;

}

// This function receives the client struct parsed as a void*, and then typecasted back to a Client*
void* receive_messages(void* clientStruct) {

	Client* client = (Client*)clientStruct;

	// Receives messages until no message was received
	while (1) {
			
			// Cleaning the buffer to avoid memory trash
			memset(buff[client->buffer], 0, MAX_MSG_SIZE);
			pthread_mutex_lock(&accept_mutex);
			// Ignore unused clients
			if (client->socket == SOCKET_ERROR)
				continue;

			bytes_read = recv(client->socket, buff[client->buffer], MAX_MSG_SIZE, 0);

			// Check for disconnecting client
			if (strcmp(buff[client->buffer], "/exit") == 0) {
				client->socket = SOCKET_ERROR;
				printf("Client %d Disconnected\n", client->buffer);
				pthread_mutex_unlock(&accept_mutex);
				break;
			}
			printf("Received %d bytes from Client %d\n", bytes_read, client->buffer);
			pthread_mutex_unlock(&accept_mutex);

		forward_messages(client);
	}
	return NULL;
}

// Sends a message to all the clients
void forward_messages(Client* client){

	for (int i = 0; i < 5; i++) {
		
		// Cleaning the buffer to avoid memory trash
		memset(buff[client->buffer], 0, MAX_MSG_SIZE);

		// Locking here to avoid receiving at the same position while sending
		pthread_mutex_lock(&accept_mutex);
		// Not send to unused clients
		if (Clients[i].socket == SOCKET_ERROR) continue;

		if (bytes_read > 0) {
			buff[client->buffer][bytes_read] = '\0';
			send(Clients[i].socket, buff[client->buffer], bytes_read, 0);
		}
		pthread_mutex_unlock(&accept_mutex);
	}
}
