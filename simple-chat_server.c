#include "simple-chat_functions.h"

void* forward_messages(void* clientSocket);

char 		buff[MAX_MSG_SIZE];
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
int 		bytes_read = 0;


int main(void) {

	// Initializes the Windows Socket API
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	pthread_t thread1;

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

	// Accepting incoming client
	SOCKET client = accept(server, NULL, NULL);

	// Creating the thread to receive and send messages while receiving new connections
	pthread_mutex_init(&print_mutex, NULL);
	pthread_create(&thread1, NULL, forward_messages, (void*) &client);

	// Joining the thread so that the program waits until there is no traffic to close
	pthread_join(thread1, NULL);

	// Closing both sockets
	closesocket(client);
	closesocket(server);

	// Windows Socket API function that cleans the socket usage and other resources, to avoid safety issues
	WSACleanup();

	return 0;
}

// The void function used on the thread
// This function receives the client socket parsed as a void*, and then typecasted back to a SOCKET*
void* forward_messages(void* clientSocket) {
	SOCKET* client = (SOCKET*)clientSocket;
	buff[bytes_read] = '\0';
	
	// Forwards messages until no message was received
	while ((bytes_read = recv(*client, buff, MAX_MSG_SIZE, 0)) > 0) {
		send(*client, buff, bytes_read, 0);
	}
	return NULL;
}
