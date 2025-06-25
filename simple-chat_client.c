#include "simple-chat_functions.h"

char buffer[MAX_MSG_SIZE];
char username[30];
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void* receive_messages(void* socket_ptr);

int main(void) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    char serverIp[INET_ADDRSTRLEN];

    printf("Insert your username:\n> ");
    scanf("%s", username);
    flush_input();


    SOCKET server = initialize_Socket_IPv4();
    struct sockaddr_in addr;
    short int connection;

    do {
        printf("Insert the server's IP:\n> ");
        scanf("%s", serverIp);
        flush_input();  // Prevent leftover \n

        addr = generate_IPv4_Address(serverIp, PORT);
        
        connection = connect(server, (struct sockaddr*)&addr, sizeof(addr));
        
        if (connection == SOCKET_ERROR) {
            system("cls");
            printf("Connection failed.\n");
        }
    } while (connection == SOCKET_ERROR);

    printf("Connected to server!\n");
    printf("+---------------------+\n\n");

    pthread_t receiver_thread;
    pthread_create(&receiver_thread, NULL, receive_messages, (void*)&server);

    char message[MAX_MSG_SIZE];
    char final_message[1054];

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

        strcpy(final_message, concat(message, username));

        if (strcmp(message, "/exit") == 0) {
            send(server, message, strlen(message), 0);
            break;
        }

        encrypt(final_message);
        send(server, final_message, strlen(final_message), 0);
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
        encrypt(buffer);  // Decrypt
        pthread_mutex_lock(&print_mutex);
        printf("%s\n", buffer);
        fflush(stdout);
        pthread_mutex_unlock(&print_mutex);
    }

    return NULL;
}
