#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSG_SIZE 512
#define PORT 5000

int main(void) {

	char buff[MAX_MSG_SIZE];
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);


	struct sockaddr_in caddr;
	struct sockaddr_in saddr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = htonl(INADDR_ANY),
		.sin_port = htons(PORT)
	};

	int server = socket(AF_INET, SOCK_STREAM, 0);
	int client, x;
	bind(server, (struct sockaddr*)&saddr, sizeof(saddr));

	listen(server, 5);

	while (1) {
		client = accept(server, (struct sockaddr*) &caddr, (int*) sizeof(caddr));
		x = recv(client, buff, MAX_MSG_SIZE, 0);
		

		//This puts will suffer changes
		printf("%s", buff);
		fflush(stdout);

		closesocket(client);
	}

	return 0;
}
