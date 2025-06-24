#include "simple-chat_functions.h"

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void* handle_client(void* arg);
void broadcast(const char* msg, int sender_id);

int main(void) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server = initialize_Socket_IPv4();
    struct sockaddr_in server_addr = generate_IPv4_Address("", PORT);

    bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server, MAX_CLIENTS);

    printf("Server listening on port %d...\n", PORT);
    getWirelessIP();

    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket = INVALID_SOCKET;
    }

    int client_id = 0;
    while (1) {
        if (client_id >= MAX_CLIENTS) break;

        SOCKET new_client = accept(server, NULL, NULL);
        if (new_client == INVALID_SOCKET) continue;

        clients[client_id].socket = new_client;
        clients[client_id].id = client_id;

        pthread_create(&clients[client_id].thread, NULL, handle_client, (void*)&clients[client_id]);

        printf("Client %d connected.\n", client_id + 1);
        client_id++;
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(clients[i].thread, NULL);
        closesocket(clients[i].socket);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}

void* handle_client(void* arg) {
    Client* client = (Client*)arg;

    while (1) {
        memset(client->buffer, 0, MAX_MSG_SIZE);
        int bytes_read = recv(client->socket, client->buffer, MAX_MSG_SIZE - 1, 0);

        if (bytes_read <= 0 || strcmp(client->buffer, "/exit") == 0) {
            printf("Client %d disconnected.\n", client->id + 1);
            closesocket(client->socket);
            client->socket = INVALID_SOCKET;
            break;
        }

        client->buffer[bytes_read] = '\0';
        broadcast(client->buffer, client->id);
    }

    return NULL;
}

void broadcast(const char* msg, int sender_id) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket != INVALID_SOCKET && i != sender_id) {
            send(clients[i].socket, msg, strlen(msg), 0);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}
