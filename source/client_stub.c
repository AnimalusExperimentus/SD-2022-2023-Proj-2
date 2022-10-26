/*
*   Grupo 12
*   Duarte Lopes Pinheiro nº54475
*   Filipe Henriques nº55228
*   Márcio Moreira nº41972
*/

#include "../include/data.h"
#include "../include/entry.h"
#include "../include/client_stub_private.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "string.h"

#include "network_client.h"
#include "client_stub.h"
#include "entry.h"
#include "data.h"


/* Remote tree. A definir pelo grupo em client_stub-private.h
 */
//sstruct rtree_t;



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


//---------------------------------------------------------------------------------------------------
/* Função para adicionar um elemento na árvore.
 * Se a key já existe, vai substituir essa entrada pelos novos dados.
 * Devolve 0 (ok, em adição/substituição) ou -1 (problemas).
 */
int rtree_put(struct rtree_t *rtree, struct entry_t *entry){

  struct message_t *msg; //create_message(MESSAGE_T__OPCODE__OP_PUT,MESSAGE_T__C_TYPE__CT_ENTRY);
  //struct _MessageT messg = MESSAGE_T__INIT;
  msg->ms->key = entry->key;
  msg->ms->data = entry->value;
  if ((msg = network_send_receive(rtree,msg))==NULL) {
    return -1;
  }

  return any_error(msg->ms->opcode,MESSAGE_T__OPCODE__OP_PUT);

}

/* Função para obter um elemento da árvore.
 * Em caso de erro, devolve NULL.
 */
struct data_t *rtree_get(struct rtree_t *rtree, char *key){

  struct message_t *msg = create_message(MESSAGE_T__OPCODE__OP_GET,MESSAGE_T__C_TYPE__CT_KEY);
  //struct _MessageT msg = MESSAGE_T__INIT;
  msg->ms->key = key;
  msg->ms->data_size = strlen(key);

  if ((msg = network_send_receive(rtree,msg))==NULL) {
    return NULL;
  }

  if (any_error(msg->ms->opcode,MESSAGE_T__OPCODE__OP_GET) != 0) {
    return NULL;
  }

  struct data_t *d = malloc(msg->ms->data_size);
  d = data_create2(msg->ms->data_size,msg->ms->key);

  return d;
}

/* Função para remover um elemento da árvore. Vai libertar
 * toda a memoria alocada na respetiva operação rtree_put().
 * Devolve: 0 (ok), -1 (key not found ou problemas).
 */
int rtree_del(struct rtree_t *rtree, char *key){

  struct message_t *msg = create_message(MESSAGE_T__OPCODE__OP_DEL,MESSAGE_T__C_TYPE__CT_KEY);
  //struct _MessageT msg = MESSAGE_T__INIT;
  msg->ms->data = key;
  msg->ms->data_size = strlen(key);

  if ((msg = network_send_receive(rtree,msg))==NULL) {
    return -1;
  }

  return any_error(msg->ms->opcode,MESSAGE_T__OPCODE__OP_DEL);
}

/* Devolve o número de elementos contidos na árvore.
 */
int rtree_size(struct rtree_t *rtree){

  struct message_t *msg = create_message(MESSAGE_T__OPCODE__OP_SIZE,MESSAGE_T__C_TYPE__CT_NONE);
  //struct _MessageT msg = MESSAGE_T__INIT;
  if ((msg = network_send_receive(rtree,msg))==NULL) {
    return -1;
  }

  if (any_error(msg->ms->opcode,MESSAGE_T__OPCODE__OP_DEL) != 0) {
    return -1;
  }

  return msg->ms->data_size;

}

/* Função que devolve a altura da árvore.
 */
int rtree_height(struct rtree_t *rtree){

  struct message_t *msg = create_message(MESSAGE_T__OPCODE__OP_HEIGHT,MESSAGE_T__C_TYPE__CT_NONE);
  //struct _MessageT msg = MESSAGE_T__INIT;
  if ((msg = network_send_receive(rtree,msg))==NULL) {
    return -1;
  }

  if (any_error(msg->ms->opcode,MESSAGE_T__OPCODE__OP_DEL) != 0) {
    return -1;
  }

  return msg->ms->data_size; // nao eh data size VERIFICAR


}

/* Devolve um array de char* com a cópia de todas as keys da árvore,
 * colocando um último elemento a NULL.
 */
char **rtree_get_keys(struct rtree_t *rtree){

  struct message_t *msg = create_message(MESSAGE_T__OPCODE__OP_GETKEYS,MESSAGE_T__C_TYPE__CT_NONE);
  //struct _MessageT msg = MESSAGE_T__INIT;

  if ((msg = network_send_receive(rtree,msg))==NULL) {
    return NULL;
  }

  if (any_error(msg->ms->opcode,MESSAGE_T__OPCODE__OP_DEL) != 0) {
    return NULL;
  }

  return msg->ms->keys;


}

/* Devolve um array de void* com a cópia de todas os values da árvore,
 * colocando um último elemento a NULL.
 */
void **rtree_get_values(struct rtree_t *rtree){

}




/*
 *Função auxiliar
 */
struct message_t *create_message(short op,short c_tp){
  struct _MessageT *ms;
  //message_t__init(&ms);
  //struct MESSAGE_T *mess = malloc(sizeof(struct MESSAGE_T));

  ms->opcode = op;
  ms->c_type = c_tp;
  struct message_t *msg = malloc(sizeof(ms));
  msg->ms = ms;
  //msg.messg.opcode = op;
  //msg.messg.c_type = c_tp;

  return msg;
}

/*
 *Função auxiliar
 */
int any_error(short op_f,short op_i){

  if (op_f == MESSAGE_T__OPCODE__OP_ERROR) {
    return -1;
  }
  return 0;
}
