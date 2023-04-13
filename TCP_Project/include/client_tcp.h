#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX_MESSAGE_LENGTH 1024

typedef struct client {
    int sockfd;
    struct sockaddr_in address;
    int addrlen;
} client_t;

int connect_to_server(client_t* client, const char* ip, int port);
void disconnect_from_server(client_t* client);
int send_message(client_t* client, const char* message);
int receive_message(client_t* client, char* buffer, int buffer_size);

#endif
