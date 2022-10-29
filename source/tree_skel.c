#include "../include/sdmessage.pb-c.h"
#include "../include/tree.h"
#include "../include/tree_skel.h"
#include "../include/data.h"
#include "../include/entry.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct tree_t *tree;


/* Inicia o skeleton da árvore.
 * O main() do servidor deve chamar esta função antes de poder usar a
 * função invoke(). 
 * Retorna 0 (OK) ou -1 (erro, por exemplo OUT OF MEMORY)
 */
int tree_skel_init() {
   
   tree = tree_create();
    if(tree == NULL) {
        return -1;
    }
    return 0;
}

/* Liberta toda a memória e recursos alocados pela função tree_skel_init.
 */
void tree_skel_destroy() {
    
    if(tree != NULL) {
        tree_destroy(tree);
    }
}

/* Executa uma operação na árvore (indicada pelo opcode contido em msg)
 * e utiliza a mesma estrutura message_t para devolver o resultado.
 * Retorna 0 (OK) ou -1 (erro, por exemplo, árvore nao incializada)
*/
int invoke(MessageT *msg) {
    
    if(msg==NULL || msg->opcode < 0 || msg->c_type < 0 || msg->opcode > MESSAGE_T__OPCODE__OP_ERROR || msg->c_type > MESSAGE_T__C_TYPE__CT_NONE) {
        return -1;
    }
    int i;
    char *key;
    switch(msg->opcode) {
        case MESSAGE_T__OPCODE__OP_SIZE:

            msg->opcode=MESSAGE_T__OPCODE__OP_SIZE+1;
            msg->c_type=MESSAGE_T__C_TYPE__CT_RESULT;
            msg->size=tree_size(tree);
            return 0;

        case MESSAGE_T__OPCODE__OP_HEIGHT:
            
            msg->opcode=MESSAGE_T__OPCODE__OP_HEIGHT+1;
            msg->c_type=MESSAGE_T__C_TYPE__CT_RESULT;
            msg->size=tree_height(tree);
            return 0;

        case MESSAGE_T__OPCODE__OP_DEL:

            key = malloc(msg->size+1);
            memset(key, '\0', msg->size+1);
            memcpy(key, msg->key, msg->size);
            
            i=tree_del(tree,key);
            if(i==0){
                msg->opcode=MESSAGE_T__OPCODE__OP_DEL+1;
            }else{
                msg->opcode=MESSAGE_T__OPCODE__OP_ERROR;
                printf("COULD NOT FIND ENTRY\n");
            }
            msg->c_type=MESSAGE_T__C_TYPE__CT_NONE;
            return 0;

        case MESSAGE_T__OPCODE__OP_GET:

            key = malloc(msg->size+1);
            memset(key, '\0', msg->size+1);
            memcpy(key, msg->key, msg->size);

            struct data_t *t = tree_get(tree, key);
            free(key);

            // TODO: WHERE TO PUT THIS?? tree_get returns NULL on not finding anything...
            // msg->opcode=MESSAGE_T__OPCODE__OP_ERROR;
            // msg->c_type=MESSAGE_T__C_TYPE__CT_NONE;
            if(t == NULL) {
                msg->opcode=MESSAGE_T__OPCODE__OP_GET+1;
                msg->c_type=MESSAGE_T__C_TYPE__CT_VALUE;
                printf("COULD NOT FIND ENTRY\n");
                msg->data.data = NULL;
                msg->data.len = 0;
                msg->size = 0;
            } else {
                msg->opcode=MESSAGE_T__OPCODE__OP_GET+1;
                msg->c_type=MESSAGE_T__C_TYPE__CT_VALUE;
                
                msg->data.data = malloc(t->datasize);
                msg->data.len = t->datasize;
                msg->size = t->datasize;
                memcpy(msg->data.data, t->data, msg->size);
            }
            data_destroy(t);
            return 0;

        case MESSAGE_T__OPCODE__OP_PUT:
            
            struct data_t *new_data = data_create((int)msg->data.len);
            memcpy(new_data->data, msg->data.data, msg->data.len);
            char* temp_key = malloc(msg->size);
            memcpy(temp_key, msg->key, msg->size);
            
            printf("%s\n", temp_key);

            int r = tree_put(tree, temp_key, new_data);
            data_destroy(new_data);
            free(temp_key);

            if(r == 0){
                msg->opcode=MESSAGE_T__OPCODE__OP_PUT+1;
            }else{
                msg->opcode=MESSAGE_T__OPCODE__OP_ERROR;
            }
            msg->c_type=MESSAGE_T__C_TYPE__CT_NONE;
            return 0;

        case MESSAGE_T__OPCODE__OP_GETKEYS:

            msg->n_keys=tree_size(tree);
            char** kk = malloc((sizeof(char**)*msg->n_keys)+sizeof(char**));
            kk = tree_get_keys(tree);
            msg->opcode=MESSAGE_T__OPCODE__OP_GETKEYS+1;
            msg->c_type=MESSAGE_T__C_TYPE__CT_KEYS;           
            memcpy(msg->keys,kk,sizeof(kk));//duvidas nao so copia endereços teria que passar por tudo novamente
           
            return 0;

        case MESSAGE_T__OPCODE__OP_GETVALUES:
            
            void **val = tree_get_values(tree);
            msg->opcode=MESSAGE_T__OPCODE__OP_GETVALUES;
            msg->c_type=MESSAGE_T__C_TYPE__CT_VALUES;
            msg->size=sizeof(val);//??
            // msg->data=?;
            return 0;

    }
    return -1;
}
