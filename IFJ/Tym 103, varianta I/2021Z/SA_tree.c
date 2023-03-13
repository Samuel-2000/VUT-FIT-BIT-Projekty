/** 
 * @file SA_tree.c
 * @brief implementace vytváření uzlů v syntaktickém stromu.. 
 * Funkce potřebné pro syntaktickou analýzu
 * @author Andrii Dovbush <xdovbu00@stud.fit.vutbr.cz>
 * @author Denis Kramár <xkrama06@stud.fit.vutbr.cz>
 */

#include "SA_tree.h"

SA_tree_node* sa_create_node() {  // vytvori obecny uzel.
	SA_tree_node* newNode = (SA_tree_node*) malloc(sizeof(SA_tree_node));
	if (newNode) {  // malloc success
		if (n_vec_init(&newNode->nodes)) {  // nastavi zakladni hodnoty do alokovaneho seznamu.
			free(newNode);  // error.
			return NULL;  // error.
		}
		newNode->parent = NULL;
		newNode->token = NULL;
		newNode->type = -1;
	}
	return newNode;
}

SA_tree_node* sa_create_term_node() {  // vytvori terminalni uzel.
	SA_tree_node* newNode = sa_create_node();  // vytvori obecny uzel.
	if (newNode) {  // uzel uspesne vytvoren.
		newNode->type = N_terminal;  // zmeni obecny uzel na terminalni.
	}
	return newNode;  // success == ukazatel na uzel. fail == NULL.
}

int n_vec_init(n_vector_t** vector) {  // nastavi pocatecni hodnoty do noveho seznamu uzlů.
	if (vector) {  // seznam existuje
		n_vector_t* newVector = (n_vector_t*) malloc(sizeof(n_vector_node_t));  // proc sizeof(n_vector_node_t) a ne sizeof(n_vector_t)?
		if (newVector) {  // malloc success
			newVector->first = NULL;
			newVector->last = NULL;
			newVector->size = 0;
			*vector = newVector;
			return 0;  // success
		}
	}
	return 1;  // malloc fail, nebo neexistujici seznam.
}

void n_vec_sa_tree_nodes_free(n_vector_t* vector) {
}

void n_vec_free(n_vector_t* vector) {  // vymaze cely seznam.
	recursive_n_vec_node_free(&vector->first);
	free(vector);
}

void recursive_n_vec_node_free(n_vector_node_t** node) {  // vymaze vsechny prvky seznamu.
	if (*node) {  // uzel existuje.
		recursive_n_vec_node_free(&(*node)->next);  // zavola dalsi uzel.
		free(*node);  // uvolni uzel.
	}
}

SA_tree_node* n_vec_push_back(n_vector_t* vector, SA_tree_node* node) {  // Vloží nový uzel na konec seznamu uzlů.	
	if (vector != NULL) {  // seznam existuje
		n_vector_node_t* newNode = (n_vector_node_t*) malloc(sizeof(n_vector_node_t));
		if (newNode != NULL) {  // malloc success
			newNode->next = NULL;
			newNode->prev = vector->last;
			newNode->node = node;
			vector->size++;
			if (vector->last != NULL) {  // neprazdny seznam
				vector->last->next = newNode;  // predchozi posledni ukazuje na novy posledni
			} else {  // prazdny seznam
				vector->first = newNode;  // prvni prvek
			}
			vector->last = newNode;  // konec seznamu ukazuje na novy uzel.
			return vector->last->node;  // ?CHYBA?  vraci to stejne co ma jako parametr
		}
	}
	return NULL;  // malloc fail, nebo neexistujici seznam.
}

SA_tree_node* n_vec_pop_back(n_vector_t* vector) {  // odstrání poslední uzel z konce seznamu uzlů.	
	if (vector != NULL) {  // seznam existuje
		if (vector->last != NULL) {  // uzel existuje
			n_vector_node_t* nodeToDelete = vector->last;  // mažeme poslední prvek.
			SA_tree_node* returnNode = nodeToDelete->node;  // vracíme uzel v posledním prvku.
			vector->size--;
			if (vector->first == vector->last) {  // mažeme jediný prvek v seznamu.
				vector->first = NULL;
				vector->last = NULL;
			}	else {  // aspoň 2 prvky v seznamu.
				vector->last = nodeToDelete->prev;  // konec seznamu ukazuje na predposledni (nově poslední) prvek.
				vector->last->next = NULL;  // predposledni (nově poslední) prvek uz nema praveho souseda.
			}
			free(nodeToDelete);  // smaže prvek.
			return returnNode;  // vrátí uzel smazaného prvku.
		}
	}
	return NULL;  // seznam nebo uzel neexistuje.
}

SA_tree_node* n_vec_get_first(n_vector_t* vector) {  // vrati prvni uzel seznamu.
	if (vector != NULL) {  // seznam existuje.
		if (vector->first) {
			return vector->first->node;
		}
	}
	return NULL;
}

SA_tree_node* n_vec_get_last(n_vector_t* vector) {  // vrati posledni uzel seznamu.
	if (vector != NULL) {  // seznam existuje.
		if (vector->last) {  // posledni prvek existuje.
			return vector->last->node;  // success
		}
	}
	return NULL;  // neexistujici seznam, nebo prvek
}

SA_tree_node* n_vec_get(n_vector_t* vector, unsigned index) {	 // vrati uzel ze seznamu na odpovidajicim indexu.
	if (vector != NULL) {  // seznam existuje.
		n_vector_node_t* iter = NULL;
		if (index < vector->size) {  // validní index.
			iter = vector->first;
			for (unsigned i = 0; i < index; i++) {  // iteruje přes pravé sousedy, až dokud není na správném indexu.
				if (iter) {  // tohle by se melo za spravne implementace dit vzdy (nepotrebny if).
					iter = iter->next;
				}
			}
			if (iter) {  // tohle by se melo za spravne implementace dit vzdy (nepotrebny if).
				return iter->node;  // success.
			}
		}
	}
	return NULL;  // neexistujici seznam / index vetsi nez velikost seznamu / chybny prvek neukazujici na dalsi (chybna implementace).
}

SA_tree_node* n_vec_insert_after_id(n_vector_t* vector, unsigned index, SA_tree_node* node) {
	if (vector != NULL) {  // seznam existuje
		n_vector_node_t* iter = NULL;
		if (index < vector->size) {  // validní index.
			iter = vector->first;
			for (unsigned i = 0; i < index; i++) {  // iteruje přes pravé sousedy, až dokud není na správném indexu.
				if (iter) {  // tohle by se melo za spravne implementace dit vzdy (nepotrebny if).
					iter = iter->next;
				}
			}
			n_vector_node_t* newNode = (n_vector_node_t*)malloc(sizeof(n_vector_node_t));
			if (newNode != NULL) {  // malloc success
				vector->size++;
				newNode->prev = iter;
				if (iter) {
					newNode->node = node;
					newNode->next = iter->next;
					if (iter->next) {
						iter->next->prev = newNode;
					}	else {
						vector->last = newNode;
					}
					iter->next = newNode;
				}	else {
					vector->first = newNode;
					vector->last = newNode;
					newNode->next = NULL;
				}
				return newNode->node;
			}
		}
	}
	return NULL;  // malloc fail, nebo neexistujici seznam.
}