#include <stdio.h>
#include <stdlib.h>
#include "server_tcp.h"

int main(int argc, char const *argv[]) {
    server_t server;

    // Inicializar el servidor en el puerto 8080
    if (init_server(&server, 8080) < 0) {
        printf("Error al inicializar el servidor\n");
        return -1;
    }

    // Escuchar conexiones entrantes y manejarlas
    handle_client_connections(&server);

    // Cerrar el servidor
    close_server(&server);

    return 0;
}
