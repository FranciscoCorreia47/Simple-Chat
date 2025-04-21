#include "simple-chat_functions.h"

char 		buff[MAX_MSG_SIZE];
int 		buff_bytes  = 0;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void* receive_messages(void* serverSocket);

//Struct that holds two variables that will be used as arguments in the function 'receive_messages' 
typedef struct {
	short int message_size;
	SOCKET 	  server;
} thread_args;

int main(void) {

	//Initializes the the Windows sockets API
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	thread_args message_info;
	pthread_t   thread1;
	char* 	    serverIp;
	char 	    username[30];

	//Gets the username the will be displayed, from the client
	printf("Insira o seu nome de utilizador:\n> ");
	scanf("%[^\n]", nome);
	short int message_size = MAX_MSG_SIZE + strlen(username) + 2;
	
	printf("Insert the server's IP?");
	scanf("%s", serverIp);

	//Creates the server socket
	message_info.server = initialize_Socket_IPv4();

	struct sockaddr_in server_address = generate_IPv4_Address(serverIp, PORT);

	/*short int checkPort = checkPort5000();
	if (checkPort) {
		Sleep(2000);
		return 1;
	}*/

	//Connects the socket with the the address and port given 
	int connectionResult = connect(server, (struct sockaddr*)&server_address, sizeof(server_address));

	//Verifies if the connection was successful or not
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

	//Creates a second thread to process the receiving of messages
	pthread_mutex_init(&print_mutex, NULL);
	pthread_create(&thread1, NULL, *receive_messages, (void *)message_info);

	char message[MAX_MSG_SIZE] = { 0 };

	//This loop gets the message from the client, encrypts it and send it to the server
	while(1) {

		memset(message, 0, sizeof(message));
		pthread_mutex_lock(&print_mutex);
		printf("Write your message: ");
		scanf("%[^\n]", message);
		pthread_mutex_unlock(&print_mutex);
		encrypt(message);
		strcpy(message, concat(message, username));
		send(server, final_message, message_size, 0);

	}

	printf("Bye!\n");
	Sleep(2000);

	// Joining the thread so that the program waits until there is no traffic to close
	pthread_join(thread1, NULL);

	// Closing the server socket
	closesocket(server);

	// Windows Socket API function that cleans the socket usage and other resources, to avoid safety issues
	WSACleanup();

	return 0;
}

// The void function used on the thread
//Funtion that receives the messages from the server, decrypts them and prints them in the screen
void* receive_messages(void* message_info) {
	buff[buff_bytes] = '\0';
	message info = (thread_args *)message_info;
	
	while ((buff_bytes = recv(*server, buff, *msg, 0)) > 0) {
		pthread_mutex_lock(&print_mutex);
		encrypt(buff);
		wprintf(L"%s\n", buff);
		fflush(stdout);
		pthread_mutex_unlock(&print_mutex);
	}
	return NULL;
}
