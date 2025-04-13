#include "simple-chat_functions.h"

void *receive_messages(void *arg);

char buff[MAX_MSG_SIZE];
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
int bytes_read = 0;

int main(void){
	pthread_t thread1;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);

	int serverFD = initialize_Socket_IPv4();
	struct sockaddr_in clientAddress;
	struct sockaddr_in *serverAddress = generate_IPv4_Address("", PORT);

	int clientFD, x;
	int result = bind(serverFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	if (result == 0)
		printf("Socket was bound successfuly!\n");
	else
		printf("Binding error!\n");

	int listenResult = listen(serverFD, 5);

	if (listenResult == 0)
		printf("Socket is listening!\n");
	else
		printf("Listening error!\n");

	int client_size = sizeof(clientAddress);
	clientFD = accept(serverFD, (struct sockaddr*) &clientAddress, &client_size);
	
	pthread_mutex_init(&print_mutex, NULL);
	pthread_create(&thread1, NULL, receive_messages, &clientFD);
	
	closesocket(clientFD);
	
	pthread_join(thread1, NULL);
	
	return 0;
}

void *receive_messages(void *arg){
	SOCKET *clientFD = (SOCKET *)arg;
	buff[bytes_read] = '\0';
	while((bytes_read = recv(*clientFD, buff, sizeof(buff), 0)) > 0){
		pthread_mutex_lock(&print_mutex);
		printf("Message: %s\n", buff);
		fflush(stdout);
		pthread_mutex_unlock(&print_mutex);


		send(*clientFD, buff, bytes_read, 0);
	}
	return NULL;
}

