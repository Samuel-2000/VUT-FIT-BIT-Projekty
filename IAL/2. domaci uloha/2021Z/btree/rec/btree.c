// Samuel Kuchta (xkucht11)
#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

void bst_init(bst_node_t **tree) {
  // printf("\n\tbst_Init\n");
  *tree = NULL;
}

bool bst_search(bst_node_t *tree, char key, int *value) {
  // printf("\n\tbst_Search\n");
  if (tree == NULL) {  // Not found.
    // printf("\n\tbst_Search null\n");
    return false;
  } else if (key == tree->key) {  // Found.
    // printf("\n\tbst_Search Found. Value: %i, Key: %c\n", *value, key);
    *value = tree->value;  // Store value.
    return true;
  } else if (key < tree->key) {  // Search left.
    // printf("\n\tbst_Search left\n");
    return bst_search(tree->left, key, value);
  } else {  // Search right.
    // printf("\n\tbst_Search right\n");
    return bst_search(tree->right, key, value);
  }
}

void bst_insert(bst_node_t **tree, char key, int value) {
  // printf("\n\tbst_Insert\n");
  if (*tree == NULL) {  // Empty tree
    // printf("\n\tbst_Insert Empty tree Value: %i, Key: %c\n", value, key);
    if (((* tree) = (bst_node_t*) malloc(sizeof(struct bst_node))) == NULL) {  // Create root.
      return;  // Failed allocation.
        // printf("\n\tbst_Insert malloc fail\n");
    }
	  (*tree)->left = (*tree)->right = NULL;  // Set parameters.
    (*tree)->key = key;
	  (*tree)->value = value;
  } else if (key == (*tree)->key) {  // Node already exists.
    // printf("\n\tbst_Insert  Node already exists. New_Value: %i, Key: %c\n", value, key);
    (*tree)->value = value;  // Set new value.
  } else if (key < (*tree)->key) {
    // printf("\n\tbst_Insert left.\n");
    bst_insert(&((*tree)->left), key, value);  // Insert left.
  } else {
    // printf("\n\tbst_Insert right.\n");
    bst_insert(&((*tree)->right), key, value);  // Insert right.
  }
}

void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  // printf("\n\tbst_replace_by_rightmost.\n");
  bst_node_t* RM;
  if ((*tree)->right == NULL) {  // Right child during first recursion doesnt exist.
    // printf("\n\tbst_replace_by_rightmost Right child during first recursion doesnt exist.\n");
    RM = *tree;
    target->key = RM->key;
    target->value = RM->value;
    target->left = RM->left;
    free(RM);
  } else if ((*tree)->right->right == NULL) {  // Last instance of recursion.
    // printf("\n\tbst_replace_by_rightmost Last instance of recursion.\n");
    RM = (*tree)->right;
    target->key = RM->key;
    target->value = RM->value;
    (*tree)->right = NULL;
    if (RM->left) {
      (*tree)->right = RM->left;
    }
    free(RM);
  } else {
    bst_replace_by_rightmost(target, & ((*tree)->right));  // zavolání funkce rekurzívně pro pravý podstrom
  }
}

void bst_delete(bst_node_t **tree, char key) {
  if (*tree != NULL) {  // Not Empty tree.
    if (key < (*tree)->key) {
      bst_delete(&((*tree)->left), key);  // Left.
    } else if (key > (*tree)->key) {
      bst_delete(&((*tree)->right), key);  // Right.
    } else {
      bst_node_t* help_ptr = *tree;  // Root.
      if (help_ptr->left && help_ptr->right) {  // Both children.
        bst_replace_by_rightmost(help_ptr, & (help_ptr->left));
        return;
      } else if (help_ptr->left) {  // Left child.
        *tree = help_ptr->left;
      } else if (help_ptr->right) {  // Right child.
        *tree = help_ptr->right;
      } else {  // No children.
        *tree = NULL; // kořenový uzel bude NULL
      }
      free(help_ptr); // uvolnění mazaného uzlu z paměti
    }
  }
}

void bst_dispose(bst_node_t **tree) {
  if (*tree != NULL) {
    bst_dispose(&((*tree)->left));  // Zavoláme funkci rekurzívně pro levý i pravý podstrom.
    bst_dispose(&((*tree)->right));
    free(*tree);  // Uvolnění ukazatele na kořen z paměti a nastavení na hodnotu NULL.
    *tree = NULL;
  }
}

void bst_preorder(bst_node_t *tree) {
  // printf("\n\tbst_preorder.\n");
  if (tree != NULL) {
    // printf("\n\tbst_preorder Not Null.\n");
    bst_print_node(tree);
    bst_preorder(tree->left);
    // printf("\n\tbst_preorder Left Done.\n");
    bst_preorder(tree->right);
    // printf("\n\tbst_preorder Right Done.\n");
  }
}

void bst_inorder(bst_node_t *tree) {
    // printf("\n\tbst_inorder.\n");
    if (tree != NULL) {
    // printf("\n\tbst_inorder Not Null.\n");
    bst_inorder(tree->left);
    // printf("\n\tbst_inorder Left Done.\n");
    bst_print_node(tree);
    bst_inorder(tree->right);
    // printf("\n\tbst_inorder Right Done.\n");
  }
}

void bst_postorder(bst_node_t *tree) {
    // printf("\n\tbst_postorder.\n");
    if (tree != NULL) {
    // printf("\n\tbst_postorder Not Null.\n");
    bst_postorder(tree->left);
    // printf("\n\tbst_postorder Left Done.\n");
    bst_postorder(tree->right);
    // printf("\n\tbst_postorder Right Done.\n");
    bst_print_node(tree);
  }
}