#include "simple-chat_functions.h"
#include "simple-chat_tweaks.h"

char buff[MAX_MSG_SIZE];
char message[MAX_MSG_SIZE];
int bytes_read = 0;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void *receive_messages(void *arg);

int main(void) {
	pthread_t thread1;
	char* ip = "127.0.0.1";
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);

	struct sockaddr_in *serverAddress = generate_IPv4_Address(ip, PORT);

	int socketFileDescriptor = initialize_Socket_IPv4();

	int connection = connect(socketFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	if (connection == 0)
		printf("Connection was successful!\n");
	else
		printf("Connection error!\n");

	pthread_mutex_init(&print_mutex, NULL);
	pthread_create(&thread1, NULL, receive_messages, &socketFileDescriptor);
	
	while(1){
		printf("> ");
		scanf("%[^\n]s", message);
		send(socketFileDescriptor, message, strlen(message), 0);
	}
	
	closesocket(socketFileDescriptor);
	
	pthread_join(thread1, NULL);
	
	return 0;
}


void *receive_messages(void *arg){
    SOCKET *socketFileDescriptor = (SOCKET *)arg;
	buff[bytes_read] = '\0';
	while((bytes_read = recv(*socketFileDescriptor, buff, sizeof(buff), 0)) > 0){
		encrypt(buff);
		pthread_mutex_lock(&print_mutex);
		printf("Received: %s\n", buff);
		fflush(stdout);
		pthread_mutex_unlock(&print_mutex);
	}
	return NULL;
}
