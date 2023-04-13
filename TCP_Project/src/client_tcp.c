#include "client_tcp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int connect_to_server(client_t* client, const char* ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        return -1;
    }

    memset(&(client->address), 0, sizeof(client->address));
    client->address.sin_family = AF_INET;
    client->address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &(client->address.sin_addr)) <= 0) {
        perror("invalid address / address not supported");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&(client->address), sizeof(client->address)) == -1) {
        perror("connection failed");
        return -1;
    }

    client->sockfd = sockfd;
    client->addrlen = sizeof(client->address);
    return 0;
}

void disconnect_from_server(client_t* client) {
    close(client->sockfd);
}

int send_message(client_t* client, const char* message) {
    int bytes_sent = send(client->sockfd, message, strlen(message), 0);
    if (bytes_sent == -1) {
        perror("send failed");
        return -1;
    }
    return 0;
}

int receive_message(client_t* client, char* buffer, int buffer_size) {
    int bytes_received = recv(client->sockfd, buffer, buffer_size, 0);
    if (bytes_received == -1) {
        perror("receive failed");
        return -1;
    }
    buffer[bytes_received] = '\0';
    return bytes_received;
}
