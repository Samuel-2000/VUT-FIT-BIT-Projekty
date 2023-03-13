#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

bool bst_search(bst_node_t *tree, char key, int *value) {
  bool search = false;
  bool finish = (tree == NULL);
  while (finish == false) {
    if (key == tree->key) {
        search = true;
        tree->value = *value;
        finish = true;
    } else {
      if (key < tree->key) {
        tree = tree->left;
      } else {
        tree = tree->right;
      }
      if (tree == NULL) {
        finish = true;
      }
    }
  }
  return search;
}

void bst_insert(bst_node_t **tree, char key, int value) {
	bst_node_t* insert_after = NULL;	// Inicializace ukazatele na uzel, za který se bude vkládat nový uzel a dalších pomocných proměnných.
  bst_node_t* node_iterator = *tree;
	while (node_iterator != NULL) {
    insert_after = node_iterator;
		if (key < node_iterator->key) { // vyhledávání v levém podstromu
			node_iterator = node_iterator->left;
		} else if (key > node_iterator->key) { // vyhledávání v pravém podstromu
			node_iterator = node_iterator->right;
		} else {
      node_iterator->value = value;
		  return; // vkládaný uzel již existuje
    }
	}
  bst_node_t* new_node;
	if ((new_node = (bst_node_t*) malloc(sizeof(struct bst_node))) == NULL) {	// vytvoření nového uzlu stromu
		return; // malloc selhal
	}
	new_node->value = value;	// nastavení parametrů nového uzlu
  new_node->key = key;
	new_node->left = new_node->right = NULL;
	if (insert_after == NULL) { // vkládaný uzel bude kořen
		*tree = new_node;
	} else if (key < insert_after->key) {  // vkládaný uzel bude levý list nalezeného uzlu
		insert_after->left = new_node;
	} else {
	  insert_after->right = new_node; // vkládaný uzel bude pravý list nalezeného uzlu
  }
}

void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
}

void bst_delete(bst_node_t **tree, char key) {
}

void bst_dispose(bst_node_t **tree) {
 /* stack_bst_t* stack;
	stack_bst_init(stack);
	bst_node_t* Help_ptr;
	while ((*tree != NULL) || stack_bst_empty(stack) == false) {
   printf("\n\tbst_dispose tree Value: %i, Key: %c\n", (*tree)->value, (*tree)->key);
		if (*tree == NULL) {
			*tree = stack_bst_pop(stack);
		}
		if ((*tree)->right != NULL) {
			stack_bst_push(stack, (*tree)->right);
		}
		Help_ptr = *tree;
		*tree = (*tree)->left;
		free(Help_ptr);
	}*/
    if (*tree != NULL) {
    bst_dispose(&((*tree)->left));  // Zavoláme funkci rekurzívně pro levý i pravý podstrom.
    bst_dispose(&((*tree)->right));
    free(*tree);  // Uvolnění ukazatele na kořen z paměti a nastavení na hodnotu NULL.
    *tree = NULL;
  }
}

void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  /*
  while (tree != NULL) {
	stack_bst_push(to_visit, tree);
	bst_print_node(tree);
	tree = tree->left;
  }
  */
}

void bst_preorder(bst_node_t *tree) {
    /*
  stack_bst_t* to_visit;
  stack_bst_init(to_visit);
  bst_leftmost_preorder(tree, to_visit);
  while (!stack_bst_empty(to_visit)) {
    tree = stack_bst_pop(to_visit);
    bst_leftmost_preorder(tree->right, to_visit);
  }
    */
}

void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
      /*
  	while (tree != NULL) {
		stack_bst_push(to_visit, tree);
		tree = tree->left;
	}
    */
}

void bst_inorder(bst_node_t *tree) {
      /*
  stack_bst_t* to_visit;
  stack_bst_init(to_visit);
	bst_leftmost_inorder(tree, to_visit);
	while (!stack_bst_empty(to_visit)) {
		tree = stack_bst_pop(to_visit);
		bst_print_node(tree); // zpracování uzlu
		bst_leftmost_inorder(tree->right, to_visit);
	}
    */
}

void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
      /*
  	while (tree) {
		stack_bst_push(to_visit, tree);
		stack_bst_push(first_visit, true); // uložení hodnoty true na zásobník (uzel byl navštíven poprvé)
		tree = tree->left;
	}
    */
}

void bst_postorder(bst_node_t *tree) {
      /*
	stack_bst_t* to_visit;
	stack_bool_t* first_visit;
  stack_bst_init(to_visit);
	stack_bst_init(first_visit);
	bst_leftmost_postorder(tree, to_visit, first_visit);	// Zpracování kořene funkcí Leftmost_Postorder, která navštívené uzly ukládá na zásobník s hodnotou true.
	while (!stack_bst_empty(to_visit)) {
		tree = stack_bst_pop(to_visit);
		if (stack_bst_pop(first_visit)) {// uzel byl navšíven pouze jednou
			stack_bst_push(to_visit, tree);
			stack_bst_push(first_visit, false);// Uložíme uzel znovu na zásobník s hodnotou false (byl navštíven podruhé)
			bst_leftmost_postorder(tree->right, to_visit, first_visit);
		} else {
		  bst_print_node(tree);  // Uzel už je navšívený podruhé, už se vracíme, proto ho zpracujeme.
    }
	}
    */
}
