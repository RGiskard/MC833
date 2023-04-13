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
    printf("Valor de database: %d\n", database);
    
    server_t server;

    // Inicializar el servidor en el puerto 8080
    if (init_server(&server, 8080,database) < 0) {
        printf("Error al inicializar el servidor\n");
        exit(1);
    }
    // Obtener el nombre del host y la dirección IP
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("Error al obtener el nombre del host");
        exit(1);
    }

    printf(hostname);

    // Escuchar conexiones entrantes y manejarlas
    handle_client_connections(&server);

    // Cerrar el servidor
    close_server(&server);
    return 0;
}

int run_client() {
    const int max_buffer_size = 1024;
    char buffer[max_buffer_size];
    client_t client;

    char ip[20];
    int port;
    
    // Pedir al usuario que ingrese la dirección IP y el puerto del servidor
    printf("Ingrese la dirección IP del servidor: ");
    fgets(ip, 20, stdin);
    printf("Ingrese el puerto del servidor: ");
    scanf("%d", &port);
    getchar(); // Limpiar el buffer de entrada
    
    // Conectar al servidor
    if (connect_to_server(&client, ip, port) < 0) {
        printf("No se pudo conectar al servidor\n");
        return -1;
    }

    // Enviar un mensaje al servidor
    printf("Ingrese el mensaje a enviar: ");
    fgets(buffer, max_buffer_size, stdin);
    if (send_message(&client, buffer) < 0) {
        printf("No se pudo enviar el mensaje al servidor\n");
        disconnect_from_server(&client);
        return -1;
    }

    // Recibir la respuesta del servidor
    if (receive_message(&client, buffer, max_buffer_size) < 0) {
        printf("No se pudo recibir la respuesta del servidor\n");
        disconnect_from_server(&client);
        return -1;
    }

    printf("Respuesta del servidor: %s\n", buffer);

    // Desconectar del servidor
    disconnect_from_server(&client);

    return 0;
}


int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <CLIENT|SERVER>\n", argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "CLIENT") == 0) {
        return run_client();
    } else if (strcmp(argv[1], "SERVER") == 0) {
        return run_server();
    } else {
        printf("Uso: %s <CLIENT|SERVER>\n", argv[0]);
        return -1;
    }
}