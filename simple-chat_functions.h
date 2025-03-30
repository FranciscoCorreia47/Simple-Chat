#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>
#include <errno.h>
#include <malloc.h>
#include <windows.h>
#include <threads.h>

#define MAX_MSG_SIZE 512
#define PORT 5000

int initialize_Socket_IPv4();

struct sockaddr_in* generate_IPv4_Address(char* ip, int port);

