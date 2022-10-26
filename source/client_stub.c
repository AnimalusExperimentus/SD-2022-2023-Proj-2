#include "../include/data.h"
#include "../include/entry.h"
#include "../include/client_stub_private.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "string.h"



/* Remote tree. A definir pelo grupo em client_stub-private.h
 */
struct rtree_t;



/* Função para estabelecer uma associação entre o cliente e o servidor, 
 * em que address_port é uma string no formato <hostname>:<port>.
 * Retorna NULL em caso de erro.
 */
struct rtree_t *rtree_connect(const char *address_port){
    if(address_port == NULL){
        return NULL;
    }

    struct rtree_t *rTree;

    if(rTree=malloc(len(address_port))==NULL)
        return NULL;

        if((rTree->sockfd=socket(AF_INET,SOCK_STREAM,0))<0){

            free(rTree);
            return NULL;

        }


        char copAdr [len(address_port)];
        strcpy(copAdr,address_port);
        char *adr=strtok(copAdr,':');
        int port=(int) strtol(strtok(NULL,'\0'),NULL,10);


        rTree->server.sin_family = AF_INET;
        rTree->server.sin_port = htons(port);

        if(inet_pton(AF_INET,adr,&rTree->server.sin_addr)<1){
            return NULL;
        }

        if ((network_connect(rTree)) == -1) {
            free(rTree);
            return NULL;
        }

    return rTree;

}

/* Termina a associação entre o cliente e o servidor, fechando a 
 * ligação com o servidor e libertando toda a memória local.
 * Retorna 0 se tudo correr bem e -1 em caso de erro.
 */
int rtree_disconnect(struct rtree_t *rtree){
        if (rtree == NULL) {
        return -1;
    }
    if (network_close(rtree) == -1) {
        return -1;
    }
    free(rtree);
    return 0;
}

/* Função para adicionar um elemento na árvore.
 * Se a key já existe, vai substituir essa entrada pelos novos dados.
 * Devolve 0 (ok, em adição/substituição) ou -1 (problemas).
 */
int rtree_put(struct rtree_t *rtree, struct entry_t *entry){

    
}

/* Função para obter um elemento da árvore.
 * Em caso de erro, devolve NULL.
 */
struct data_t *rtree_get(struct rtree_t *rtree, char *key);

/* Função para remover um elemento da árvore. Vai libertar 
 * toda a memoria alocada na respetiva operação rtree_put().
 * Devolve: 0 (ok), -1 (key not found ou problemas).
 */
int rtree_del(struct rtree_t *rtree, char *key);

/* Devolve o número de elementos contidos na árvore.
 */
int rtree_size(struct rtree_t *rtree);

/* Função que devolve a altura da árvore.
 */
int rtree_height(struct rtree_t *rtree);

/* Devolve um array de char* com a cópia de todas as keys da árvore,
 * colocando um último elemento a NULL.
 */
char **rtree_get_keys(struct rtree_t *rtree);

/* Devolve um array de void* com a cópia de todas os values da árvore,
 * colocando um último elemento a NULL.
 */
void **rtree_get_values(struct rtree_t *rtree);
