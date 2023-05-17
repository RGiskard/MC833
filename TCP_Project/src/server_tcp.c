#include "server_tcp.h"

int init_server(server_t *server, int port,int database)
{
    // Criar o socket do servidor
    if ((server->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error al crear el socket del servidor");
        return -1;
    }

    // Configurar endereço e porta do servidor
    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(port);
    server->addrlen = sizeof(server->address);
    server->database;

    // Associar o socket do servidor ao endereço e porta
    if (bind(server->server_fd, (struct sockaddr *)&server->address, sizeof(server->address)) < 0)
    {
        perror("Error ao associar o socket do servidor ao endereço e porta");
        return -1;
    }

    // Escutar conexões entrantes.
    if (listen(server->server_fd, 3) < 0)
    {
        perror("Erro ao escutar conexões entrantes.");
        return -1;
    }

    // Inicializar o array de sockets de clientes.
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        server->client_sockets[i] = 0;
    }
    server->max_clients = MAX_CLIENTS;

    printf("Servidor iniciado na porta %d\n Salvando em banco de dados %d\n", port, database);
    return 0;
}

void handle_client_connections(server_t *server)
{
    fd_set readfds;
    int max_sd, sd, activity, new_socket, valread;
    struct sockaddr_in client_address;
    char buffer[1024] = {0};

    // Limpar o conjunto de descritores de arquivo
    FD_ZERO(&readfds);

    // Adicionar o descritor de arquivo do servidor ao conjunto
    FD_SET(server->server_fd, &readfds);
    max_sd = server->server_fd;

    while (1)
    {
        // Aguardar por atividade em algum dos sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            perror("Error on select");
        }

// Se houver atividade no socket do servidor, é uma nova conexão de entrada
        if (FD_ISSET(server->server_fd, &readfds))
        {
            if ((new_socket = accept(server->server_fd, (struct sockaddr *)&client_address, (socklen_t *)&server->addrlen)) < 0)
            {
                perror("Erro ao aceitar nova conexão");
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

            // Se o array de sockets de clientes estiver cheio
            if (i == server->max_clients)
            {
                printf("Se alcanzó el número máximo de clientes. Rechazando nueva conexión.\n");
                close(new_socket);
            }

            // Adicionar o novo socket ao conjunto de descritores de arquivo (file descriptors)
            FD_SET(new_socket, &readfds);
            // Atualizar o descritor de arquivo máximo
            if (new_socket > max_sd)
            {
                max_sd = new_socket;
            }
        }

        // Percorrer os sockets dos clientes para verificar se há atividade em algum deles
        int i, sd;
        for (i = 0; i < server->max_clients; i++)
        {
            sd = server->client_sockets[i];

            if (FD_ISSET(sd, &readfds))
            {
                // Se houver atividade no socket, é uma solicitação de dados
                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    // O client fecho a conexão
                    printf("Cliente desconectado, socket fd es %d, dirección es: %s, puerto: %d\n", sd, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                    // Fechar o socket e removê-lo do conjunto de descritores de arquivo (file descriptors)
                    close(sd);
                    FD_CLR(sd, &readfds);
                    server->client_sockets[i] = 0;
                }
                else
                {
                    // Show message
                    printf("Mensaje recibido: %s\n", buffer);
                    // Enviar uma resposta ao cliente
                    send(sd, buffer, strlen(buffer), 0);
                    // clean the buffer
                    memset;
                }

               // Verificar todas as conexões dos clientes em busca de atividade
                for (int i = 0; i < server->max_clients; i++)
                {
                    sd = server->client_sockets[i];

                    if (FD_ISSET(sd, &readfds))
                    {
                        // Se houver atividade no socket, é uma mensagem de entrada
                        if ((valread = read(sd, buffer, 1024)) == 0)
                        {
                            // O cliente fechou a conexão, remover o socket da matriz de clientes
                            getpeername(sd, (struct sockaddr *)&client_address, (socklen_t *)&server->addrlen);
                            printf("Host desconectado, endereco: %s, port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                            close(sd);
                            server->client_sockets[i] = 0;
                        }
                        else
                        {
                            // Processar a mensagem recebida
                            buffer[valread] = '\0';
                            printf("Mensaje recibido: %s\n", buffer);
                            // Aqui você pode implementar a lógica da sua aplicação para processar a mensagem
                        }
                    }
                }
            }
        }
    }
}

void close_server(server_t *server)
{
    // Fechar todos os sockets dos clientes
    for (int i = 0; i < server->max_clients; i++)
    {
        int client_socket = server->client_sockets[i];
        if (client_socket != 0)
        {
            close(client_socket);
        }
    }

    // Fechar o socket do servidor
    close(server->server_fd);

    printf("Servidor cerrado\n");
}
