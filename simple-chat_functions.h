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

// The default message size
#define MAX_MSG_SIZE 1024
// The network port used by the app
#define PORT 5000

// Definition of the functions declared on simple-chat_functions.c
int checkPort5000();
int getWirelessIP();
SOCKET initialize_Socket_IPv4();
struct sockaddr_in generate_IPv4_Address(char* ip, int port);
