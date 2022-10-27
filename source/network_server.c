#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdbool.h>
#include "../include/tree_skel.h"
#include "../include/network-private.h"

#define SIZE_CLIENT_DEFAULT 1024
int listen_number;

typedef MessageT message_t;

/* Função para preparar uma socket de receção de pedidos de ligação
 * num determinado porto.
 * Retornar descritor do socket (OK) ou -1 (erro).
 */
int network_server_init(short port){
    if (port < 0) {
        perror("Port value is invalid");
        return -1;
    }

    int socket_fd;
    struct sockaddr_in *server, optional;

     if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Something went wrong");
        return -1;
    }

    server = malloc(sizeof(struct sockaddr_in));

    server->sin_family = AF_INET;
    server->sin_port = htons(port);
    server->sin_addr.s_addr = htonl(INADDR_ANY);

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optional, sizeof(optional)) < 0) {
        perror("Something went wrong - SO_REUSEADDR");
        close(socket_fd);
        return -1;
    }

      if (bind(socket_fd, (struct sockaddr *) server, sizeof(struct sockaddr_in)) < 0) {
        perror("Something went wrong - bind server");
        close(socket_fd);
        return -1;
    }

      if (listen(socket_fd, 0) < 0) {
        perror("Something went wrong - listen server");
        close(socket_fd);
        return -1;
    }

     free(server);
    return socket_fd;
}


/* Esta função deve:
 * - Ler os bytes da rede, a partir do client_socket indicado;
 * - De-serializar estes bytes e construir a mensagem com o pedido,
 *   reservando a memória necessária para a estrutura message_t.
 */
message_t *network_receive(int client_socket) {
    if(client_socket<0){
        perror("Invalid Socket");
        return NULL;
    }

    unsigned buffer_size;

    
    
    read(client_socket, &buffer_size, sizeof(uint32_t));

    unsigned response_size = ntohl(buffer_size);

    char *buffer_reader = malloc(response_size);

    // We put the message in messageT_response
    if ((read_all(client_socket, buffer_reader, (int) response_size)) < 0) {
        close(client_socket);
        return NULL;
    }



    MessageT *messageT_deserialized = NULL;
    messageT_deserialized = message_t__unpack(NULL, response_size, (const uint8_t *) buffer_reader);


    free(buffer_reader);

    return messageT_deserialized;
}


/* Esta função deve:
 * - Serializar a mensagem de resposta contida em msg;
 * - Libertar a memória ocupada por esta mensagem;
 * - Enviar a mensagem serializada, através do client_socket.
 */
int network_send(int client_socket, MessageT *msg) {
     if (client_socket < 0 || msg == NULL) {
        perror("Client socket or message tried to send is invalid");
        return -1;
    }

    size_t buffer_size_host_server = message_t__get_packed_size(msg);
    unsigned buffer_size = htonl(buffer_size_host_server);

     uint8_t *buffer_write = malloc(buffer_size);
    // Serialization using protobuf
    message_t__pack(msg, buffer_write);

    write(client_socket, &buffer_size, sizeof(uint32_t));

     if (write_all(client_socket, (char *) buffer_write, (int) buffer_size_host_server) < 0) {
        free(msg);
        close(client_socket);
        return -1;
    }

    free(buffer_write);
    return 0;
}


/* A função network_server_close() liberta os recursos alocados por
 * network_server_init().
 */
int network_server_close() {
    if (close(listen_number) == 0) {
        return 0;
    }
    return -1;
}


/* Esta função deve:
 * - Aceitar uma conexão de um cliente;
 * - Receber uma mensagem usando a função network_receive;
 * - Entregar a mensagem de-serializada ao skeleton para ser processada;
 * - Esperar a resposta do skeleton;
 * - Enviar a resposta ao cliente usando a função network_send.
 */
int network_main_loop(int listening_socket) {
    struct sockaddr client;
    int client_socket;
    socklen_t size_client = SIZE_CLIENT_DEFAULT;

    if (listening_socket < 0) {
        perror("Listening socket is invalid");
        return -1;
    }

    listen_number=listening_socket;

    while(true)
    {
        if((client_socket=accept(listening_socket, &client, &size_client))!=-1) {
            
            message_t *messageT = network_receive(client_socket);
            
            if (messageT == NULL || messageT->opcode == MESSAGE_T__OPCODE__OP_BAD ||
                messageT->c_type == MESSAGE_T__C_TYPE__CT_BAD) {
                continue;
            } else {
                invoke(messageT);
                network_send(client_socket, messageT);
            }
            close(client_socket);
        }
    }
    return 0;
}