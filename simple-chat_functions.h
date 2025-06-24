#ifndef SIMPLE_CHAT_FUNCTIONS_H
#define SIMPLE_CHAT_FUNCTIONS_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple-chat_tweaks.h"

#define PORT 5000
#define MAX_MSG_SIZE 1024
#define MAX_CLIENTS 5

typedef struct {
    SOCKET socket;
    int id;
    pthread_t thread;
    char buffer[MAX_MSG_SIZE];
} Client;

SOCKET initialize_Socket_IPv4(void);
struct sockaddr_in generate_IPv4_Address(char* ip, int port);
void flush_input(void);
int getWirelessIP();

#endif
