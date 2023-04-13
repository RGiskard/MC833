#include "server_tcp.h"

int init_server(server_t *server, int port,int database)
{
    // Crear socket del servidor
    if ((server->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error al crear el socket del servidor");
        return -1;
    }

    // Configurar dirección y puerto del servidor
    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(port);
    server->addrlen = sizeof(server->address);
    server->database;

    // Asociar el socket del servidor a la dirección y puerto
    if (bind(server->server_fd, (struct sockaddr *)&server->address, sizeof(server->address)) < 0)
    {
        perror("Error al asociar el socket del servidor a la dirección y puerto");
        return -1;
    }

    // Escuchar conexiones entrantes
    if (listen(server->server_fd, 3) < 0)
    {
        perror("Error al escuchar conexiones entrantes");
        return -1;
    }

    // Inicializar el arreglo de sockets de clientes
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        server->client_sockets[i] = 0;
    }
    server->max_clients = MAX_CLIENTS;

    printf("Servidor iniciado en el puerto %d\n Salvando em banco de dados %d\n", port,database);

    return 0;
}

void handle_client_connections(server_t *server)
{
    fd_set readfds;
    int max_sd, sd, activity, new_socket, valread;
    struct sockaddr_in client_address;
    char buffer[1024] = {0};

    // Limpiar el set de file descriptors
    FD_ZERO(&readfds);

    // Agregar el socket del servidor al set
    FD_SET(server->server_fd, &readfds);
    max_sd = server->server_fd;

    while (1)
    {
        // Esperar por actividad en alguno de los sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            perror("Error en select");
        }

        // Si hay actividad en el socket del servidor, es una nueva conexión entrante
        if (FD_ISSET(server->server_fd, &readfds))
        {
            if ((new_socket = accept(server->server_fd, (struct sockaddr *)&client_address, (socklen_t *)&server->addrlen)) < 0)
            {
                perror("Error al aceptar nueva conexión");
                continue;
            }

            printf("Nueva conexión, socket fd es %d, dirección es: %s, puerto: %d\n", new_socket, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            // Agregar el nuevo socket al arreglo de sockets de clientes
            int i;
            for (i = 0; i < server->max_clients; i++)
            {
                // Si la posición está vacía
                if (server->client_sockets[i] == 0)
                {
                    server->client_sockets[i] = new_socket;
                    printf("Agregando socket del cliente a la lista de sockets: %d\n", i);
                    break;
                }
            }

            // Si el arreglo de sockets de clientes está lleno
            if (i == server->max_clients)
            {
                printf("Se alcanzó el número máximo de clientes. Rechazando nueva conexión.\n");
                close(new_socket);
            }

            // Agregar el nuevo socket al set de file descriptors
            FD_SET(new_socket, &readfds);
            // Actualizar el descriptor de archivo máximo
            if (new_socket > max_sd)
            {
                max_sd = new_socket;
            }
        }

        // Recorrer los sockets de clientes para ver si hay actividad en alguno
        int i, sd;
        for (i = 0; i < server->max_clients; i++)
        {
            sd = server->client_sockets[i];

            if (FD_ISSET(sd, &readfds))
            {
                // Si hay actividad en el socket, es una solicitud de datos
                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    // El cliente cerró la conexión
                    printf("Cliente desconectado, socket fd es %d, dirección es: %s, puerto: %d\n", sd, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                    // Cerrar el socket y quitarlo del set de file descriptors
                    close(sd);
                    FD_CLR(sd, &readfds);
                    server->client_sockets[i] = 0;
                }
                else
                {
                    // Mostrar el mensaje recibido
                    printf("Mensaje recibido: %s\n", buffer);
                    // Enviar una respuesta al cliente
                    send(sd, buffer, strlen(buffer), 0);
                    // Limpiar el buffer
                    memset;
                }

                // Revisar todas las conexiones de clientes en busca de actividad
                for (int i = 0; i < server->max_clients; i++)
                {
                    sd = server->client_sockets[i];

                    if (FD_ISSET(sd, &readfds))
                    {
                        // Si hay actividad en el socket, es un mensaje entrante
                        if ((valread = read(sd, buffer, 1024)) == 0)
                        {
                            // El cliente ha cerrado la conexión, eliminar el socket del arreglo de clientes
                            getpeername(sd, (struct sockaddr *)&client_address, (socklen_t *)&server->addrlen);
                            printf("Host desconectado, dirección: %s, puerto: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                            close(sd);
                            server->client_sockets[i] = 0;
                        }
                        else
                        {
                            // Procesar el mensaje recibido
                            buffer[valread] = '\0';
                            printf("Mensaje recibido: %s\n", buffer);
                            // Aquí puedes implementar la lógica de tu aplicación para procesar el mensaje
                        }
                    }
                }
            }
        }
    }
}

void close_server(server_t *server)
{
    // Cerrar todos los sockets de clientes
    for (int i = 0; i < server->max_clients; i++)
    {
        int client_socket = server->client_sockets[i];
        if (client_socket != 0)
        {
            close(client_socket);
        }
    }

    // Cerrar el socket del servidor
    close(server->server_fd);

    printf("Servidor cerrado\n");
}
