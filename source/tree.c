/*
*   Grupo 12
*   Duarte Lopes Pinheiro nº54475
*   Filipe Henriques nº55228
*   Márcio Moreira nº41972
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/tree.h"
#include "../include/entry.h"
#include "../include/data.h"
#include "../include/tree-private.h"



/* Função para criar uma nova árvore tree vazia.
 * Em caso de erro retorna NULL.
 */
struct tree_t *tree_create(){
	struct tree_t *tree;
	tree =malloc(sizeof(struct tree_t));
    return tree;
}


/* Função para libertar toda a memória ocupada por uma árvore.
 */
void tree_destroy(struct tree_t *tree){
	if (tree != NULL){
		tree_destroy(tree->left);
		tree_destroy(tree->right);
		entry_destroy(tree->nodeEntry);
		free(tree);
	}

}

/** Função para adicionar um par chave-valor à árvore.
 * Os dados de entrada desta função deverão ser copiados, ou seja, a
 * função vai *COPIAR* a key (string) e os dados para um novo espaço de
 * memória que tem de ser reservado. Se a key já existir na árvore,
 * a função tem de substituir a entrada existente pela nova, fazendo
 * a necessária gestão da memória para armazenar os novos dados.
 * Retorna 0 (ok) ou -1 em caso de erro.
 */
int tree_put(struct tree_t *tree, char *key, struct data_t *value) {

	struct entry_t *temp;

	if(key == NULL || value == NULL){
		return -1;
	}

	if(tree == NULL){
		return -1;
	}

	temp = entry_create(strdup(key), data_dup(value));

	if (tree -> nodeEntry == NULL) {
		tree -> nodeEntry = temp;
		tree -> left = NULL;
		tree -> right = NULL;
		return 0;

	}

	int comprare = entry_compare(temp,tree -> nodeEntry);

	if(comprare == 0) {
		entry_replace(tree -> nodeEntry,key,value);
		return 0;
	}

	if(comprare == -1) {
		if(tree->left == NULL) {
			tree->left = tree_create();
			tree->left -> nodeEntry  = temp;
			tree -> left -> left = NULL;
			tree -> left -> right = NULL;
			return 0;
		}

		return tree_put(tree -> left,key,value);
	}

	if(comprare == 1) {
		if(tree -> right == NULL) {
			tree -> right = tree_create();
			tree -> right -> nodeEntry = temp;
			tree -> right -> left = NULL;
			tree -> right -> right = NULL;
			return 0;
		}

		return tree_put(tree->right,key,value);
	}
	return -1;
}

/* Função para obter da árvore o valor associado à chave key.
 * A função deve devolver uma cópia dos dados que terão de ser
 * libertados no contexto da função que chamou tree_get, ou seja, a
 * função aloca memória para armazenar uma *CÓPIA* dos dados da árvore,
 * retorna o endereço desta memória com a cópia dos dados, assumindo-se
 * que esta memória será depois libertada pelo programa que chamou
 * a função. Devolve NULL em caso de erro.
 */
struct data_t *tree_get(struct tree_t *tree, char *key){
	if(tree == NULL || tree -> nodeEntry == NULL || key == NULL){
		return NULL;
	}
	struct data_t *d;
	int comp = strcmp(key, tree->nodeEntry->key);
	if (comp == 0){
		d = data_dup(tree ->nodeEntry -> value);
		return d;
	}
	else if(comp < 0){
		return tree_get(tree -> left, key);
	}
	else if(comp > 0){
		return tree_get(tree -> right, key);
	}
	return NULL;
}


/* Função para remover um elemento da árvore, indicado pela chave key,
 * libertando toda a memória alocada na respetiva operação tree_put.
 * Retorna 0 (ok) ou -1 (key not found).
 */
int tree_del(struct tree_t *tree, char *key){
	struct entry_t *temp = entry_create(key, NULL);
	struct entry_t *root = tree -> nodeEntry;
	int result;
	if(tree -> left == NULL&&tree->right==NULL){
		entry_destroy(temp);
		return -1;
	}

	if(entry_compare(root, temp) == -1){
		result = tree_del(tree -> right, key);
	}else if(entry_compare(root, temp) == -1){
		result = tree_del(tree -> left, key);
	}else{
		entry_destroy(tree->nodeEntry->value);
		return 0;
	}
	
	return result;
}

/* Função que devolve o número de elementos contidos na árvore.
 */
int tree_size(struct tree_t *tree) {
	int count=0;
	if(tree == NULL || tree -> nodeEntry == NULL)
		return 0;
    else {
        count =  tree_size(tree -> left) + 1 + tree_size(tree -> right);
        return count;
    }
}

/* Função que devolve a altura da árvore.
 */
int tree_height(struct tree_t *root) {
  // Root being null means tree doesn't exist.
  if (root == NULL)
    return 0;

  // Get the depth of the left and right subtree
  // using recursion.
  int leftDepth = tree_height(root->left);
  int rightDepth = tree_height(root->right);

  // Choose the larger one and add the root to it.
  if (leftDepth > rightDepth)
    return leftDepth + 1;
  else
    return rightDepth + 1;
}

/* Função que devolve um array de char* com a cópia de todas as keys da
 * árvore, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária. As keys devem vir ordenadas segundo a ordenação lexicográfica das mesmas.
 */
char **tree_get_keys(struct tree_t *tree){
	if(tree == NULL || tree -> nodeEntry == NULL){
		return NULL;
	}
	printf("PASSOU O NULL\n");
	int size = tree_size(tree);
	char **tree_keys =  malloc(sizeof(char *) *(size+1));

	
	struct entry_t *entry;
	printf("passou outro NULL\n");

	tree_keys = inOrder(tree_keys, tree, 0);




	return tree_keys;
}

/* Função que devolve um array de void* com a cópia de todas os values da
 * árvore, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária.
 */
void **tree_get_values(struct tree_t *tree){
	if(tree == NULL || tree -> nodeEntry == NULL){
		return NULL;
	}
	printf("PASSOU O NULL\n");
	int size = tree_size(tree);
	char **tree_keys =  malloc(sizeof(struct  data_t) *(size+1));

	
	struct entry_t *entry;
	printf("passou outro NULL\n");

	tree_keys = inOrder(tree_keys, tree, 0);


	return NULL;
}





/* Função que liberta toda a memória alocada por tree_get_keys().
 */
void tree_free_keys(char **keys){
	if (keys != NULL) {
		int i = 0;
		while (keys[i] != NULL) {
			free(keys[i]);
			i++;
		}
		free(keys);
	}
}

/* Função que liberta toda a memória alocada por tree_get_values().
 */
void tree_free_values(void **values){
	if (values != NULL) {
		int i = 0;
		while (values[i] != NULL) {
			free(values[i]);
			i++;
		}
		free(values);
	}
}
