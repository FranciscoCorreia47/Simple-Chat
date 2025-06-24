#include "simple-chat_functions.h"

char buffer[MAX_MSG_SIZE];
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void* receive_messages(void* socket_ptr);

int main(void) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    char serverIp[INET_ADDRSTRLEN];
    printf("Insert the server's IP:\n> ");
    scanf("%s", serverIp);
    flush_input();  // Prevent leftover \n

    SOCKET server = initialize_Socket_IPv4();
    struct sockaddr_in addr = generate_IPv4_Address(serverIp, PORT);

    if (connect(server, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Connection failed.\n");
        closesocket(server);
        WSACleanup();
        return 1;
    }

    printf("Connected to server!\n");

    pthread_t receiver_thread;
    pthread_create(&receiver_thread, NULL, receive_messages, (void*)&server);

    char message[MAX_MSG_SIZE];
    while (1) {
        printf("You> ");
        pthread_mutex_lock(&print_mutex);
        if (fgets(message, MAX_MSG_SIZE, stdin)) {
            size_t len = strlen(message);
            if (len > 0 && message[len - 1] == '\n') {
                message[len - 1] = '\0';
            }
        }
        pthread_mutex_unlock(&print_mutex);

        if (strcmp(message, "/exit") == 0) {
            send(server, message, strlen(message), 0);
            break;
        }

        //encrypt(message);
        send(server, message, strlen(message), 0);
        Sleep(100);
    }

    pthread_join(receiver_thread, NULL);
    closesocket(server);
    WSACleanup();
    return 0;
}

void* receive_messages(void* socket_ptr) {
    SOCKET* server = (SOCKET*)socket_ptr;

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(*server, buffer, MAX_MSG_SIZE - 1, 0);

        if (bytes <= 0) break;

        buffer[bytes] = '\0';
        //encrypt(buffer);  // Decrypt
        pthread_mutex_lock(&print_mutex);
        printf("[Server]> %s\n", buffer);
        fflush(stdout);
        pthread_mutex_unlock(&print_mutex);
    }

    return NULL;
}
