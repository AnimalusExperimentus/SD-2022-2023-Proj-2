#include "../include/sdmessage.pb-c.h"
#include "../include/tree.h"
#include "../include/tree_skel.h"
#include "../include/data.h"
#include "../include/entry.h"

#include <stdlib.h>
#include <string.h>

struct tree_t *tree;


/* Inicia o skeleton da árvore.
 * O main() do servidor deve chamar esta função antes de poder usar a
 * função invoke(). 
 * Retorna 0 (OK) ou -1 (erro, por exemplo OUT OF MEMORY)
 */
int tree_skel_init(){
   tree=tree_create();
    if(tree==NULL){
        return -1;
    }
    return 0;
}

/* Liberta toda a memória e recursos alocados pela função tree_skel_init.
 */
void tree_skel_destroy(){
    if(tree!=NULL){
        tree_destroy(tree);
    }
    
}

/* Executa uma operação na árvore (indicada pelo opcode contido em msg)
 * e utiliza a mesma estrutura message_t para devolver o resultado.
 * Retorna 0 (OK) ou -1 (erro, por exemplo, árvore nao incializada)
*/
int invoke(MessageT *msg){
    

    if(msg==NULL || msg->opcode < 0 || msg->c_type < 0 || msg->opcode > MESSAGE_T__OPCODE__OP_ERROR || msg->c_type > MESSAGE_T__C_TYPE__CT_NONE) {
        return -1;
    }
    int i;
    switch(msg->opcode){
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
            i=tree_del(tree,msg->key);
            if(i==0){
            msg->opcode=MESSAGE_T__OPCODE__OP_DEL+1;
            }else{
            msg->opcode=MESSAGE_T__OPCODE__OP_ERROR;
            }
            msg->c_type=MESSAGE_T__C_TYPE__CT_NONE;
            return 0;

        case MESSAGE_T__OPCODE__OP_GET:
            char* key = malloc(sizeof(msg->key));
            strcpy(key,msg->key);
            struct data_t *t = tree_get(tree,key);

            ProtobufCBinaryData d ;
            d.len = t->datasize;
            d.data = malloc(t->datasize);



            if(t == NULL){
                msg->opcode=MESSAGE_T__OPCODE__OP_ERROR;
                msg->c_type=MESSAGE_T__C_TYPE__CT_NONE;
            }else{
                msg->opcode=MESSAGE_T__OPCODE__OP_GET+1;
                msg->c_type=MESSAGE_T__C_TYPE__CT_VALUE;
                memcpy(d.data,t->data,t->datasize);
                msg->data=d;
            }
            return 0;

        case MESSAGE_T__OPCODE__OP_PUT:

            struct data_t *nt = data_create((int)msg->size);

            memcpy(nt->data, msg->data.data, nt->datasize);

            //put copia coisas de daria para guardar do outro lado
            int i=tree_put(tree, msg->key, nt);

            if(i == 0){
                msg->opcode=MESSAGE_T__OPCODE__OP_PUT+1;
            }else{
                msg->opcode=MESSAGE_T__OPCODE__OP_ERROR;
            }
            msg->c_type=MESSAGE_T__C_TYPE__CT_NONE;
            return 0;

        case MESSAGE_T__OPCODE__OP_GETKEYS:
            char** kk = tree_get_keys(tree);
            msg->opcode=MESSAGE_T__OPCODE__OP_GETKEYS+1;
            msg->c_type=MESSAGE_T__C_TYPE__CT_KEYS;
            msg->n_keys=tree_size(tree);
            msg->keys=kk;
            return 0;

        case MESSAGE_T__OPCODE__OP_GETVALUES:
            void **val = tree_get_values(tree);
            msg->opcode=MESSAGE_T__OPCODE__OP_GETVALUES;
            msg->c_type=MESSAGE_T__C_TYPE__CT_VALUES;
            msg->size=sizeof(val);
            // msg->data=?;
            return 0;

        case MESSAGE_T__OPCODE__OP_BAD:
            // TODO
            return 0;

        case MESSAGE_T__OPCODE__OP_ERROR:
            // TODO
            return 0;

        case _MESSAGE_T__OPCODE_IS_INT_SIZE:
            // TODO
            return 0;
    }
    

return -1;
}
