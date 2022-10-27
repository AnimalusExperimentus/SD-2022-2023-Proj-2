/*
*   Grupo 12
*   Duarte Lopes Pinheiro nº54475
*   Filipe Henriques nº55228
*   Márcio Moreira nº41972
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tree_skel.h" // remove after test
#include "../include/network_server.h"


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Server takes 1 argument: port\n");
        exit(-1);
    }
    int port;
    if(sscanf(argv[1], "%i", &port) != 1) {
        printf("Port must be an integer\n");
        exit(-1);
    }

    printf("port: %i\n", port);

    int listening_socket = network_server_init((short)port);
    if (listening_socket == -1) {
        printf("socket creation error\n");
        exit(-1);
    }

    tree_skel_init();

    // // Test size command
    // MessageT msg;
    // message_t__init(&msg);
    // msg.opcode = MESSAGE_T__OPCODE__OP_SIZE;
    // msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;
    // if(invoke(&msg)!= 0) {
    //     perror("error on tree operation");
    // }

    // int32_t size = msg.size;
    // printf("Size is %d\n", size);

    // msg.opcode = MESSAGE_T__OPCODE__OP_PUT;
    // msg.c_type = MESSAGE_T__C_TYPE__CT_ENTRY;
    // msg.key = "produto-1928495";
    // char str[] = "Descricao do produto";
    // msg.data.data = malloc(strlen(str));
    // memcpy(msg.data.data, str, strlen(str));
    // msg.size = strlen(str);
    // if(invoke(&msg)!= 0) {
    //     perror("error on tree operation");
    // }

    // msg.opcode = MESSAGE_T__OPCODE__OP_SIZE;
    // msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;
    // msg.key = NULL;
    // msg.data.data = NULL;
    // if(invoke(&msg)!= 0) {
    //     perror("error on tree operation");
    // }

    // size = msg.size;
    // printf("Size is %d\n", size);


    int result = network_main_loop(listening_socket);
    
    tree_skel_destroy();
    exit(result);
}