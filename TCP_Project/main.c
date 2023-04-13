#include <stdio.h>
#include <stdlib.h>
#include "server_tcp.h"
#include "utils.h"

int main(int argc, char const *argv[]) {
    int database = read_config_file();
    printf("database connect = %d\n", conect());
    desconect();
    printf("Valor de database: %d\n", database);
    
    server_t server;

    // Inicializar el servidor en el puerto 8080
    if (init_server(&server, 8080,database) < 0) {
        printf("Error al inicializar el servidor\n");
        return -1;
    }

    // Escuchar conexiones entrantes y manejarlas
    handle_client_connections(&server);

    // Cerrar el servidor
    close_server(&server);

    return 0;
}
