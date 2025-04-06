#include "simple-chat_functions.h"

void *receive_messages(void *clientFD);

char buff[MAX_MSG_SIZE];
pthread_mutex_t = print_mutex;

int main(void){
	pthread_t = thread1;
	int bytes_read = 0;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);

	int serverFD = initialize_Socket_IPv4();
	struct sockaddr_in clientAddress;
	struct sockaddr_in *serverAddress = generate_IPv4_Address("", PORT);

	int clientFD, x;
	int result = bind(serverFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	if (result)
		printf("Socket was bound successfuly!\n");
	else
		printf("Binding error!\n");

	int listenResult = listen(serverFD, 5);

	if (listenResult)
		printf("Socket is listening!\n");
	else
		printf("Listening error!\n");

	clientFD = accept(serverFD, (struct sockaddr*) &clientAddress, (int*) sizeof(clientAddress));
	
	pthread_mutex_init(&print_mutex, NULL);
	pthread_create(&thread1, NULL, receive_messages, &clientFD);

	while(bytes_read > 0){
		send(clientFD, buff, bytes_read, 0);
	}
	
	closesocket(clientFD);
	
	pthread_join(thread1, NULL);
	
	return 0;
}

void *receive_messages(void *clientFD){
	buff[bytes_read] = '\0';
	while((bytes_read = recv(clientFD, buff, sizeof(buff), 0)) > 0){
		pthread_mutex_lock(&print_mutex);
		printf("Message: %s\n", buff);
		fflush(stdout);
		printf_mutex_unlock(&printf_mutex);
	}
	return NULL;
}

