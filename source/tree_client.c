/*
*   Grupo 12
*   Duarte Lopes Pinheiro nº54475
*   Filipe Henriques nº55228
*   Márcio Moreira nº41972
*/

#include "../include/client_stub.h"
#include "../include/entry.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFERLEN 4096
struct rtree_t *rtree;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Client takes 1 argument: <Server>:<Port>\n");
        exit(-1);
    }

    const char *addr = strtok(argv[1], " \n");  
    
    // Establish connection with server
    rtree = rtree_connect(addr);
    if (rtree == NULL) {
        perror("Error connecting to server\n");
        exit(-1);
    }
    printf("Connected to server!\n");

    char line[BUFFERLEN];
    while(fgets(line, BUFFERLEN, stdin)) {

        char* command = strtok(line, " \n");
        if (strcmp(command, "quit") == 0) {
            rtree_disconnect(rtree);
            break;
        // PUT -----------------------------------------------------
        } else if (strcmp(command, "put") == 0) {
            char* key = strtok(NULL, " ");
            char* data = strtok(NULL, "\n");
            if (key == NULL || data == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: put <key> <data>\n");
                continue;
            }

            struct data_t *d = data_create(strlen(data));
            memcpy(d->data, data, strlen(data));
            struct entry_t *e = entry_create(key, d);
            int r = rtree_put(rtree, e);
            entry_destroy(e);

            if (r == 0) { 
                printf("Entry added to tree\n"); 
            } else {
                printf("Error on put\n"); 
            }
        // GET -----------------------------------------------------
        } else if (strcmp(command, "get") == 0) {
            char* key = strtok(NULL, " \n");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: get <key>\n");
                continue;
            }

            struct data_t *d = rtree_get(rtree, key);
            if (d == NULL) { 
                printf("Error in get\n");
                continue;
            }
            if (d->datasize == 0 ){
                printf("Could not find key in tree\n");
            } else {
                printf("Value found for this key: %.*s\n", d->datasize, (char*)d->data);
            }

        } else if (strcmp(command, "del") == 0) {
            char* key = strtok(NULL, " ");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: del <key>\n");
                continue;
            }
            // printf("%s\n",key);
            //TODO
            else {
              printf("Error on del\n");
            }
        // SIZE -----------------------------------------------------
        } else if (strcmp(command, "size") == 0) {
            if (strtok(NULL, " ") != NULL){
                printf("syntax: size\n");
                continue;
            }

            int r = rtree_size(rtree);
            if (r == -1) {
                perror("Error on size\n");
                exit(-1);
            }

            printf("Tree size is currently %i\n", r);

        // HEIGHT -----------------------------------------------------
        } else if (strcmp(command, "height") == 0) {
            if (strtok(NULL, " ") != NULL){
                printf("syntax: height\n");
                continue;
            }

            int r = rtree_height(rtree);
            if (r == -1) {
                perror("Error on height\n");
                exit(-1);
            }

            printf("Tree height is currently %i\n", r);

        } else if (strcmp(command, "getkeys") == 0) {
            char* key = strtok(NULL, " ");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: getkeys <key>\n");
                continue;
            }
            // printf("%s\n",key);
            //TODO
            else {
              printf("Error on getkeys\n");
            }
        } else if (strcmp(command, "getvalues") == 0) {
            char* key = strtok(NULL, " ");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: getvalues <key>\n");
                continue;
            }
            // printf("%s\n",key);
            //TODO
            else {
              printf("Error on getvalues\n");
            }
        } else {
            printf("That command doesn't exist.\n");
        }
    }
    exit(0);
}
