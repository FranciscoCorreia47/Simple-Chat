#include "simple-chat_functions.h"

int main(void) {
	int bytes_read = 0;
	char buff[MAX_MSG_SIZE];
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
	
	while((bytes_read = recv(clientFD, buff, MAX_MSG_SIZE, 0)) > 0){
		printf("Message: %s\n", buff);
		send(clientFD, buff, bytes_read, 0);
	}
	
	closesocket(clientFD);


	return 0;
}
