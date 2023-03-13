/** 
 * @file syntakticka_analyza.h
 * @brief implementace parseru. 
 * Funkce potřebné pro syntaktickou analýzu
 * @author Andrii Dovbush <xdovbu00@stud.fit.vutbr.cz>
 * @author Denis Kramár <xkrama06@stud.fit.vutbr.cz>
 */

#ifndef _SYNTAKTICKA_ANALYZA_H
  #define _SYNTAKTICKA_ANALYZA_H
  #include "semanticka_analyza.h"
  #include "generator_mezikodu.h"



  #define MANAGE_TERM \
		PUSH_TERM         \
		GET_TOKEN

  #define MANAGE_NON_TERM(_node_name, _func_name)  \
    PUSH_NON_TERM(_node_name)                      \
    CALL_FUN(_func_name)

  #define MANAGE_NON_TERM_TOKEN(_node_name, _func_name) \
    PUSH_NON_TERM(_node_name)                           \
    GET_TOKEN                                           \
    CALL_FUN(_func_name)

  #define MANAGE_NON_TERM_ROOT(_node_name, _func_name)  \
    PUSH_NON_TERM(_node_name)                           \
    CALL_FUN_ROOT(_func_name)

  #define CALL_FUN_ROOT(_func_name)	\
    _func_name(root);				        \
    if (exit_code != SUCCESS) {	    \
      return;										    \
    }

  #define CALL_FUN(_func_name)	\
    _func_name(add_node);				\
    if (exit_code != SUCCESS) {	\
      return;										\
    }

  #define PUSH_TERM   \
    CREATE_TERM_NODE  \
    APPEND_CHILD_NODE

  #define PUSH_NON_TERM(_non_term_type)   \
    CREATE_NON_TERM_NODE(_non_term_type)  \
    APPEND_CHILD_NODE

    // vytvoreni ne-terminalniho uzlu non_term_type
  #define CREATE_NON_TERM_NODE(_non_term_type) 	  \
    add_node = create_node(root, _non_term_type);

    // vytvoreni terminalniho uzlu
  #define CREATE_TERM_NODE							  \
    add_node = create_terminal_node(root);

    // pridani synovskeho uzlu do seznamu uzlů.
  #define APPEND_CHILD_NODE																						\
    if (add_node && n_vec_push_back(root->nodes, add_node) == NULL) { \
      exit_code = INTERNAL_ERR; 																			\
      printf("\n\tchyba v APPEND_CHILD_NODE\n"); 											\
      return; 																												\
    }

    // nacte jeden token, a v pripade chyby, vyskoci ven ze syntakticke analyzy, (exit_code pri chybe nastavila uz LA).
  #define GET_TOKEN 					                    \
    token = (LA_token*)malloc(sizeof(LA_token));  \
    if (token) {                                  \
      if (get_token(token) == false) {            \
        return;                                   \
      }                                           \
    } else {                                      \
      exit_code = INTERNAL_ERR;                   \
      return;                                     \
    }

    // vytvori obecny neterminalni uzel.
  #define CREATE_NODE_PTR(_var)             \
    SA_tree_node* _var = sa_create_node();  \
    if (_var == NULL) {                     \
      exit_code = INTERNAL_ERR;             \
      return;                               \
    }
    

    // deklarace funkcí
  void parser();  // vytvori strom (Syntax) -> semanticka analyza -> generator kodu

  SA_tree_node* create_terminal_node(SA_tree_node* root);
  SA_tree_node* create_node(SA_tree_node* root, tree_node_type type);

    // tvoreni SA_tree pomoci gramatiky.
  void create_tree(SA_tree* tree);  // start_node
    void Program(SA_tree_node* root);
      void Global_def(SA_tree_node* root);
        void Global_type(SA_tree_node* root);
          void Type_def(SA_tree_node* root);
            void Def_arg_list(SA_tree_node* root);
              void Def_arg_list_inner(SA_tree_node* root);
              void Arg_def(SA_tree_node* root);
                bool Arg_type();
      void Function_def(SA_tree_node* root);
        void Return_list(SA_tree_node* root);
          void Types_list(SA_tree_node* root);
            void Types_list_inner(SA_tree_node* root);
          void Code(SA_tree_node* root);
            void Line_of_code(SA_tree_node* root);
              void Identifier_contin(SA_tree_node* root);
                void Identifier_list_inner(SA_tree_node* root);
            void If_block(SA_tree_node* root);
              void Else_block(SA_tree_node* root);
            void While_block(SA_tree_node* root);
            void Local_def(SA_tree_node* root);
            void Return_statement(SA_tree_node* root);
              void Expression_list(SA_tree_node* root);
                void Expression_list_inner(SA_tree_node* root);
    void Main_start(SA_tree_node* root);

    // from  precedend
  void Str_len_arg(SA_tree_node* root);
  void Condition(SA_tree_node* root);
  void Expression(SA_tree_node* root);
  
#endif  // _SYNTAKTICKA_ANALYZA_H