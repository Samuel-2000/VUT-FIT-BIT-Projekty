/** 
 * @file SA_tree.h
 * @brief implementace vytváření uzlů v syntaktickém stromu.. 
 * Funkce potřebné pro syntaktickou analýzu
 * @author Andrii Dovbush <xdovbu00@stud.fit.vutbr.cz>
 * @author Denis Kramár <xkrama06@stud.fit.vutbr.cz>
 */

#ifndef _SA_TREE_H_
  #define _SA_TREE_H_
  #include "lexikalni_analyza.h"

  typedef enum {  // typ uzlu (neterminaly + terminaly).
    N_start_node,     // S -> key_require + string + program + main_func + main_start + EoF
      N_program,        // global_def + program | function_def + program | eps
        N_global_def,   // key_global + identifier + definition + global_type
          N_global_type,  //  key_string + type_def | key_integer + type_def |  key_number + type_def | key_function + left_bracket + def_arg_list + right_bracket + return_list | eps
            N_type_def,     //  assignment + expression | eps
            N_def_arg_list,  // arg_def + def_arg_list_inner | eps
              N_def_arg_list_inner,  // comma + arg_def + def_arg_list_inner | eps
              N_arg_def,  // identifier + definition + arg_type
                N_arg_type,  // key_string | key_integer | key_number
        N_function_def,  // key_function + identifier + left_bracket + def_arg_list + right_bracket + return_list + function_body + key_end
          N_return_list,  // definition + types_list | eps
            N_types_list,  // arg_type + types_list_inner
              N_types_list_inner,  // comma + arg_type + types_list_inner | eps
            N_code,           // if_block + code | line_of_code + code | local_def + code | while_block + code | eps
              N_line_of_code, // identifier + identifier_contin
                N_identifier_contin,  // identifier_list_inner + assignment + expression_list | left_bracket + expression_list + right_bracket
                  N_identifier_list_inner,  // comma + identifier + identifier_list_inner | eps
              N_if_block,       // key_if + condition + key_then + code + else_block + key_end
                N_else_block,   // key_else + code | eps
              N_while_block,  // key_while + condition + key_do + code + key_end
              N_local_def,  // key_local + identifier + definition + arg_type + type_def
            N_return_statement,  // key_return + expression_list | eps
              N_expression_list,  // expression + expression_list_inner | eps
                N_expression_list_inner,  // comma + expression + expression_list _inner | eps
      N_main_func,
      N_main_start,

    N_arg_list,       // --------------- //
    N_arg_list_cont,  // from  precedend //
    N_str_len_arg,    // --------------- //

    N_to_integer,     // from semantic //
    N_to_number,      // from semantic //

    N_expression,  // Neterminal ktery vede k zmene typu analyzy
    N_function_call,
    N_condition,  // Neterminal ktery vede k zmene typu analyzy

    N_terminal
  } tree_node_type;  // uzly z gramatiky.


  typedef struct TreeNode         SA_tree_node;
  typedef struct SATree           SA_tree;
  typedef struct Node_Vector_Node n_vector_node_t;
  typedef struct Node_Vector      n_vector_t;

  struct SATree {  // syntakticky strom, ktery se vytvori, a pak se bude pocas jeho prochazeni kontrolovat semantika, a generovat mezikod.
    SA_tree_node* N_start_node; // ukazatel na korenovy uzel syntakticke analyzy.
  };

  struct TreeNode {       // uzel syntaktickeho stromu.
    SA_tree_node* parent; // otcovsky uzel.
    tree_node_type type;  // typ uzlu.
    n_vector_t* nodes;    // seznam synovskych uzlů.
    LA_token* token;      // v pripade terminalu (jejich typ, a data).
  };

  struct Node_Vector_Node { // uzel v seznamu
    SA_tree_node* node;     // ukazatel na uzel.
    n_vector_node_t* next;  // ukazatel na dalsi uzel v seznamu.
    n_vector_node_t* prev;  // ukazatel na predesli uzel v seznamu.
  };

  struct Node_Vector {      // seznam uzlů.
    n_vector_node_t* first; // ukazatel na prvni uzel.
    n_vector_node_t* last;  // ukazatel na posledni uzel.
    unsigned size;
  };

    // vytvaření jednotlivých uzlů.
  SA_tree_node* sa_create_node();  // vytvori obecny uzel.
  SA_tree_node* sa_create_term_node();  // vytvori terminalni uzel

    // inicializace, a smazani vektoru uzlů.
  int n_vec_init(n_vector_t** vector);  // nastavi pocatecni hodnoty do noveho seznamu uzlů.
  void n_vec_sa_tree_nodes_free(n_vector_t* vector);
  void n_vec_free(n_vector_t* vector);  // vymaze cely seznam.
  void recursive_n_vec_node_free(n_vector_node_t** node);  // vymaze vsechny prvky seznamu.

    // vkládání/odebírání uzlu na konec/konci vektoru uzlů.
  SA_tree_node* n_vec_push_back(n_vector_t* vector, SA_tree_node* node);  // Vloží nový uzel na konec seznamu uzlů.	
  SA_tree_node* n_vec_pop_back(n_vector_t* vector);  // odstrání poslední uzel z konce seznamu uzlů.	

    // vyhledani a vracení uzlu
  SA_tree_node* n_vec_get_first(n_vector_t* vector);  // vrati prvni uzel seznamu.
  SA_tree_node* n_vec_get_last(n_vector_t* vector);  // vrati posledni uzel seznamu.
  SA_tree_node* n_vec_get(n_vector_t* vector, unsigned index);	// vrati uzel ze seznamu na odpovidajicim indexu.

#endif // !_SA_TREE_H

