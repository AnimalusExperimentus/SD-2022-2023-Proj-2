#include "../include/client_stub.h"
#include "../include/sdmessage.pb-c.h"
#include "../include/client_stub_private.h"
#include "../include/network-private.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef MessageT message_t;

/* Esta função deve:
 * - Obter o endereço do servidor (struct sockaddr_in) a base da
 *   informação guardada na estrutura rtree;
 * - Estabelecer a ligação com o servidor;
 * - Guardar toda a informação necessária (e.g., descritor do socket)
 *   na estrutura rtree;
 * - Retornar 0 (OK) ou -1 (erro).
 */
int network_connect(struct rtree_t *rtree){

 if (rtree == NULL) {
        return -1;
    }

    if ((rtree->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error - socket");
        return -1;
    }

     if (connect(rtree->sockfd, (const struct sockaddr *) &rtree->server, sizeof(rtree->server)) < 0) {
        perror("connect");
        close(rtree->sockfd);
        return -1;
    }
    return 0;
}


/* Esta função deve:
 * - Obter o descritor da ligação (socket) da estrutura rtree_t;
 * - Serializar a mensagem contida em msg;
 * - Enviar a mensagem serializada para o servidor;
 * - Esperar a resposta do servidor;
 * - De-serializar a mensagem de resposta;
 * - Retornar a mensagem de-serializada ou NULL em caso de erro.
 */
message_t *network_send_receive(struct rtree_t * rtree, message_t *msg){

    if (rtree == NULL || msg == NULL) {
        perror("Client socket or message tried to send is invalid");
        return NULL;
    }
    
    unsigned buffer_size_host = message_t__get_packed_size(msg);
    unsigned buffer_size = htonl(buffer_size_host);

    void *buffer_write = malloc(buffer_size);
    // Serialization using protobuf
    message_t__pack(msg, buffer_write);

    write(rtree->sockfd,&buffer_write,sizeof(unsigned));

       if (write_all(rtree->sockfd, (char *) buffer_write, (int) buffer_size_host) < 0) {
        free(msg);
        close(rtree->sockfd);
        return NULL;
    }

    free(buffer_write);

    unsigned buffer_size_client;

    // First we received the size of the message
    read(rtree->sockfd, &buffer_size_client, sizeof(uint32_t));

    unsigned response_size = ntohl(buffer_size_client);

    char *buffer_reader = malloc(response_size);

    // We put the message in messageT_response
    if ((read_all(rtree->sockfd, (char *) buffer_reader, (int) response_size)) != response_size) {
        free(msg);
        close(rtree->sockfd);
        return NULL;
    }

    message_t *messageT_deserialized = NULL;
    //message_t__unpack(NULL, response_size, (const uint8_t *) buffer_reader);
    messageT_deserialized = message_t__unpack(NULL, response_size, (const uint8_t *) buffer_reader);


    // Finally, free all memory allocated
    free(buffer_reader);
    return messageT_deserialized;
}
                                       

/* A função network_close() fecha a ligação estabelecida por
 * network_connect().
 */
int network_close(struct rtree_t * rtree){
     if (close(rtree->sockfd) == -1) {
        return -1;
    }
    free(rtree);
    return 0;
}

