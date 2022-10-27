/*
*   Grupo 12
*   Duarte Lopes Pinheiro nº54475
*   Filipe Henriques nº55228
*   Márcio Moreira nº41972
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFERLEN 4096

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Client takes 1 argument: <Server>:<Port>\n");
        exit(-1);
    }

    char* server = strtok(argv[1], ":");
    char* port = strtok(NULL, ":");
    if (server == NULL || port == NULL) {
        printf("Arguments are: <Server>:<Port>\n");
        exit(-1);
    }
    // printf("%s + %s\n", server, port);

    char line[BUFFERLEN];
    while(fgets(line, BUFFERLEN, stdin)) {

        // printf("%s\n", line);
        char* command = strtok(line, " \n");
        if (strcmp(command, "quit") == 0)
            break;

        if(strcmp(command, "put") == 0) {
            char* key = strtok(NULL, " ");
            char* data = strtok(NULL, "\n");
            if (key == NULL || data == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: put <key> <data>\n");
                continue;
            }
            // printf("%s %s\n",key, data);
            //TODO

        } else if (strcmp(command, "get") == 0) {
            char* key = strtok(NULL, " ");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: get <key>\n");
                continue;
            }
            // printf("%s\n",key);
            //TODO

        } else if (strcmp(command, "del") == 0) {
            char* key = strtok(NULL, " ");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: del <key>\n");
                continue;
            }
            // printf("%s\n",key);
            //TODO

        } else if (strcmp(command, "size") == 0) {
            char* key = strtok(NULL, " ");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: size <key>\n");
                continue;
            }
            // printf("%s\n",key);
            //TODO
        } else if (strcmp(command, "height") == 0) {
            char* key = strtok(NULL, " ");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: height <key>\n");
                continue;
            }
            // printf("%s\n",key);
            //TODO
        } else if (strcmp(command, "getkeys") == 0) {
            char* key = strtok(NULL, " ");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: getkeys <key>\n");
                continue;
            }
            // printf("%s\n",key);
            //TODO
        } else if (strcmp(command, "getvalues") == 0) {
            char* key = strtok(NULL, " ");
            if (key == NULL || strtok(NULL, " ") != NULL){
                printf("syntax: getvalues <key>\n");
                continue;
            }
            // printf("%s\n",key);
            //TODO
        } else {
            printf("That command doesn't exist.\n");
        }
    }
    exit(0);
}
