
/*
*   Grupo 12
*   Duarte Lopes Pinheiro nº54475
*   Filipe Henriques nº55228
*   Márcio Moreira nº41972
*/

#include "../include/serialization.h"

#include "../include/data.h"
#include "../include/entry.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* Serializa todas as keys presentes no array de strings keys
 * para o buffer keys_buf que será alocado dentro da função.
 * O array de keys a passar em argumento pode ser obtido através 
 * da função tree_get_keys. Para além disso, retorna o tamanho do
 * buffer alocado ou -1 em caso de erro.
 */
int keyArray_to_buffer(char **keys, char **keys_buf){

    if(keys==NULL){
        return -1;
    }
    keys_buf=malloc(sizeof(keys));

    for(int i=0;i<sizeof(keys);i++){
        
        memcpy(keys_buf[i], keys[i], keys[i]+1);


    }

    return sizeof(keys);
}

/* De-serializa a mensagem contida em keys_buf, com tamanho
 * keys_buf_size, colocando-a e retornando-a num array char**,
 * cujo espaco em memória deve ser reservado. Devolve NULL
 * em caso de erro.
 */
char** buffer_to_keyArray(char *keys_buf, int keys_buf_size){
    char **keyArray;
    keyArray=malloc(sizeof(char**)*(strlen(keys_buf)/keys_buf_size));


    for(int i=0;i<strlen(keys_buf)/keys_buf_size;i++){
        memcpy(keyArray[i],keys_buf[i*2],keys_buf_size);

    }

    return keyArray;



}
