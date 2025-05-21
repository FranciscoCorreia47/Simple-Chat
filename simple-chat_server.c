#include "simple-chat_functions.h"

void* receive_messages(void* clientSocket);
void* forward_messages(void* clientSocket);

char 		buff[5][MAX_MSG_SIZE];
int 		bytes_read = 0;
pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
	// Initializes the Windows Socket API
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

	// Initializa sockets as -1 to avoid using memory trash
	SOCKET client[5] = {-1, -1, -1, -1, -1};

	// Creating the thread to receive and send messages while receiving new connections
	pthread_create(&thread1, NULL, receive_messages, (void*)&client);
	pthread_create(&thread2, Null, forward_messages, (void*)&client);

	// Accept incoming client connections
	int count = 0;
	while (count < 5) {
		for (int i = 0; i < 5; i++) {
			if (client[i] != SOCKET_ERROR)
				continue;
			else {
				pthread_mutex_lock(&accept_mutex);
				client[i] = accept(server, NULL, NULL);
				pthread_mutex_unlock(&accept_mutex);
				printf("Client %d Conected\n", i);
				count++;
			}
		}
	}


	// Joining the thread so that the program waits until there is no traffic to close
	pthread_join(thread1, NULL);

	// Closing both sockets
	for (int i = 0; i < 5; i++)
		closesocket(client[i]);
	
	closesocket(server);

	// Windows Socket API function that cleans the socket usage and other resources, to avoid safety issues
	WSACleanup();

	return 0;

}
// The void function used on the thread
// This function receives the client socket parsed as a void*, and then typecasted back to a SOCKET*
void* receive_messages(void* clientSocket) {
	
	SOCKET* client[5];

	for (int i = 0; i < 5; i++)
		client[i] = (SOCKET*)clientSocket+i;

	// Forwards messages until no message was received
	while (1) {
		for (int i = 0; i < 5; i++) {
			Sleep(500);
			memset(buff[i], 0, sizeof(buff));
			if (*client[i] == SOCKET_ERROR)
				continue;
			pthread_mutex_lock(&accept_mutex);
			bytes_read = recv(*client[i], buff[i], MAX_MSG_SIZE, 0);
			if (strcmp(buff[i], "/exit") == 0)
				break;
			pthread_mutex_unlock(&accept_mutex);
		}
	}
	
	return NULL;
}

void* forward_messages(void* clientSocket){
	while (1) {
		for (int i = 0; i < 5; i++) {
			Sleep(500);
			pthread_mutex_lock(&accept_mutex);
			buff[i][bytes_read] = '\0';
			send(*client[i], buff[i], sizeof(buff[i]), 0);
			pthread_mutex_unlock(&accept_mutex);
			memset(buff[i], 0, sizeof(buff));
			if (*client[i] == SOCKET_ERROR)
				continue;
		}
	}
	
	return NULL;
}
