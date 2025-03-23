#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>

#define MAX_MSG_SIZE 512
#define PORT 5000

int main(void) {

	char message[MAX_MSG_SIZE];

	char* ip = "127.0.0.1";
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);

	struct sockaddr_in saddr = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT)
	};

	inet_pton(AF_INET, ip, &saddr.sin_addr.s_addr);

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	int connection = connect(socketfd, (struct sockaddr*)&saddr, sizeof(saddr));

	if (connection == 0)
		printf("Connection was successful!\n");
	else
		printf("Connection error!\n");
	
	printf("> ");
	scanf("%s", message);
	send(socketfd, message, strlen(message), 0);

	closesocket(socketfd);

	return 0;
}
