/*
*   Grupo 12
*   Duarte Lopes Pinheiro nº54475
*   Filipe Henriques nº55228
*   Márcio Moreira nº41972
*/
#include "../include/data.h"
#include <stdlib.h>
#include <string.h>

/* Função que cria um novo elemento de dados data_t, reservando a memória
 * necessária para armazenar os dados, especificada pelo parâmetro size 
 */
struct data_t *data_create(int size){
    struct data_t *data;
    data=malloc(sizeof(struct data_t));
    if(size>0){

        data->datasize=size;
        data->data=malloc(size);
        return data;

    }
    free(data);
    return NULL;


}

/* Função que cria um novo elemento de dados data_t, inicializando o campo
 * data com o valor passado no parâmetro data, sem necessidade de reservar
 * memória para os dados.
 */
struct data_t *data_create2(int size, void *data){
    struct data_t *dataa;
    dataa=malloc(sizeof(struct data_t));
    if(size>0&&data!=NULL){
        dataa->datasize=size;
        dataa->data=malloc(size);
        dataa->data=data;
        return dataa;
    }
    free(dataa);
    return NULL;
}

/* Função que elimina um bloco de dados, apontado pelo parâmetro data,
 * libertando toda a memória por ele ocupada.
 */
void data_destroy(struct data_t *data){
    if(data!=NULL){    
    free(data->data);
    free(data);
    }
}

/* Função que duplica uma estrutura data_t, reservando toda a memória
 * necessária para a nova estrutura, inclusivamente dados.
 */
struct data_t *data_dup(struct data_t *data){

    if(data==NULL||data->datasize<1||data->data==NULL){
   
    return NULL;
    }

    struct data_t *ptr;
    ptr = data_create(data->datasize);
    
    memcpy(ptr->data,data->data,data->datasize);
    return ptr;
}

/* Função que substitui o conteúdo de um elemento de dados data_t.
*  Deve assegurar que destroi o conteúdo antigo do mesmo.
*/
void data_replace(struct data_t *data, int new_size, void *new_data){
    data_destroy(data);
    data=data_create2(new_size,new_data);
}


