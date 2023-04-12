#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_CLIENTS 10

typedef struct server {
    int server_fd;
    struct sockaddr_in address;
    int addrlen;
    int client_sockets[MAX_CLIENTS];
    int max_clients;
} server_t;

int init_server(server_t* server, int port);
void handle_client_connections(server_t* server);
void close_server(server_t* server);

#endif /* SERVER_H */
