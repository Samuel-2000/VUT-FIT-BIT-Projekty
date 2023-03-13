/** 
 * @file symtable.c
 * @brief tabulka symbolů 
 * Funkce potřebné pro tabulku symbolů
 * @author Daria Dmitrievich <xdmitr00@stud.fit.vutbr.cz>
 */

#include "symtable.h"

void symTabInit(psTree *tree) {  // Inicializuje tabulku symbolů (binární vyhledávací strom)
  *tree = NULL;
}

psData symTabSearch(psTree* tree, char* key) {  // Podle klíče vyhledá uzel ve stromě 
  if (tree != NULL) {
    while (*tree != NULL) {
      int compare = strcmp(key, (*tree)->key);
      if (compare < 0) {
        tree = &(*tree)->lptr;
      } else if (compare > 0) {
        tree = &(*tree)->rptr;
      } else {
        return (*tree)->data;
      }
    }
  }
  return NULL;
}

void symTabInsert(psTree *tree, char *key, psData data) {  // Vloží do stromu nový uzel s hodnotou data
	if (tree != NULL) {
    psTree *node = tree;
    while (*node != NULL) {
      int compare = strcmp(key, (*node)->key);
      if (compare < 0) 
        node = &(*node)->lptr;
      else if (compare > 0) 
        node = &(*node)->rptr;
      else {  // uzel uz existuje
        free((*node)->data);
        (*node)->data = data;
        return;
      }
    }
    psTree newTree;
    if ((newTree = (psTree) malloc(sizeof(struct sTree))) == NULL) {
      printf("\n\nchyba mallocu v symTabInsert\n\n");
      return;
    }  // Vloží do stromu nový uzel s hodnotou data
    newTree->data = data;
    newTree->key = key;
    newTree->lptr = NULL;
    newTree->rptr = NULL;
    *node = newTree;
  }
}

void symTabDefvarPre(psTree tree) {  // Zadefinuje všechny proměnné ve stromu a inicializuje na nil
  psStack stack;
  symStackInit(&stack);
  symTabLefmostPre(tree, stack);
  while (stack->top >= 0) {
    tree = symStackPop(stack);
    symTabLefmostPre(tree->rptr, stack);
  }
  symStackDispose(&stack);
}

void symTabLefmostPre(psTree tree, psStack stack) {  // Projde levou stranu stromu a po cestě definuje proměnné
  while (tree != NULL) {
    symStackPush(stack, tree);
    printf("DEFVAR LF@%s\nMOVE LF@%s nil@nil\n", tree->key, tree->key);
    tree = tree->lptr;
  }
}

void symTabDispose(psTree* tree) {  // Zruší celý strom (tabulku symbolů)
  if (tree != NULL && (*tree) != NULL) {
		symTabDispose(&(*tree)->lptr);// Průchod stromem
		symTabDispose(&(*tree)->rptr);
		if ((*tree)->data->localFrame != NULL) {  // Uvolnění dat
			symTabDispose(&(*tree)->data->localFrame);
		}
		free((*tree)->data);
		free(*tree);  // Uvolnění uzlu
		*tree = NULL;
	}
}

void symStackInit(psStack* stack) {  // Inicializace zásobníku.
  if (stack != NULL) {
    if ((*stack = (psStack) malloc(sizeof(struct sStack))) == NULL) {
      printf("\n\nchyba mallocu v symStackInit\n\n");
      return;
    }
		(*stack)->top = -1;
		(*stack)->size = 0;
		(*stack)->S = NULL;
	}
}

void symStackPush(psStack stack, psTree tree) {  // Vloží položku na vrchol zásobníku
  if (stack != NULL) {
		stack->top++;
		if (stack->top >= stack->size) {
			stack->size += SYMTABLE_STACK_CHUNK;
			if ((stack->S = (psTree*) realloc(stack->S, sizeof(psTree) * stack->size)) == NULL) {
        printf("\n\nchyba rellocu v symStackPush\n\n");
        return;
      }
		}
		stack->S[stack->top] = tree;
	}
}

psTree symStackPop(psStack stack) {  // Vyjme položku z vrcholu zásobníku (NULL, pokud je zásobník prázdný)
  if (stack != NULL && stack->top >= 0) {
    stack->top--;
    return stack->S[stack->top + 1];  // stack->top-- ?
  } else {
    return NULL;
  }
}

void symStackDispose(psStack* stack) {  // Uvolní zásobník z paměti
  if (stack != NULL) {
    free((*stack)->S);
    free(*stack);
    *stack = NULL;
  }
}