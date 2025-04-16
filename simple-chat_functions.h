#ifndef UNICODE
#define UNICODE
#endif


#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <malloc.h>
#include <pthread.h>
#include "simple-chat_tweaks.h"

#pragma comment(lib, "WS2_32.lib")

#define MAX_MSG_SIZE 512
#define PORT 50000

int checkPort5000();
int getWirelessIP();
SOCKET initialize_Socket_IPv4();
struct sockaddr_in generate_IPv4_Address(char* ip, int port);


