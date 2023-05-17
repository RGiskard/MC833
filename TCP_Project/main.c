#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server_tcp.h"
#include "client_tcp.h"
#include "utils.h"

int run_server() {
    int database = read_config_file();
    printf("database connect = %d\n", conect());
    desconect();
    printf("Value of database: %d\n", database);
    
    server_t server;

    // Inicializar o servidor na porta 8080
    if (init_server(&server, 8080,database) < 0) {
        printf("Error initializing the server\n");
        exit(1);
    }
    // Obter o nome do host e o endereço IP
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("Error retrieving the hostname");
        exit(1);
    }

    printf(hostname);

    // Ouvir conexões 
    handle_client_connections(&server);

    // fechar o server
    close_server(&server);
    return 0;
}

int run_client() {
    const int max_buffer_size = 1024;
    char buffer[max_buffer_size];
    client_t client;

    char ip[20];
    int port;
    
    // Pedir ao usuário que digite o endereço IP e a porta do servidor
    printf("Please enter the server's IP address: ");
    fgets(ip, 20, stdin);
    printf("Please enter the server's port: ");
    scanf("%d", &port);
    getchar(); // Limpar o buffer 
    
    // Conectar ao servidor
    if (connect_to_server(&client, ip, port) < 0) {
        printf("Failed to connect to the server\n");
        return -1;
    }

    // Enviar um mensagem ao servidor
    printf("Please enter the message to send: ");
    fgets(buffer, max_buffer_size, stdin);
    if (send_message(&client, buffer) < 0) {
        printf("Failed to send the message to the server\n");
        disconnect_from_server(&client);
        return -1;
    }

    // Receber a resposta do servidor
    if (receive_message(&client, buffer, max_buffer_size) < 0) {
        printf("Failed to receive the response from the server\n");
        disconnect_from_server(&client);
        return -1;
    }

    printf("Server response: %s\n", buffer);

    // Desconectar do servidor
    disconnect_from_server(&client);

    return 0;
}


int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <CLIENT|SERVER>\n", argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "CLIENT") == 0) {
        return run_client();
    } else if (strcmp(argv[1], "SERVER") == 0) {
        return run_server();
    } else {
        printf("Usage: %s <CLIENT|SERVER>\n", argv[0]);
        return -1;
    }
}
