/** 
 * @file semanticka_analyza.h
 * @brief semanticka analýza
 * Funkce potřebné pro sémantickou analýzu
 * @author Andrii Dovbush <xdovbu00@stud.fit.vutbr.cz>
 * @author Denis Kramár <xkrama06@stud.fit.vutbr.cz>
 */

#ifndef SEMANTICKA_ANALYZA_H
  #define SEMANTICKA_ANALYZA_H
  #include "symtable.h"
  #include "SA_tree.h"
  #include "generator_mezikodu.h"


  #define GENERATE_CODE(_fun, ...)		\
	  if (_fun(__VA_ARGS__) == false) { \
      exit_code = INTERNAL_ERR;       \
      return;                         \
    }

  // deklarace funkcí

  int semanticka_analyza(SA_tree* tree);
  psData initNodeData();

  void check_program(SA_tree_node* node);
    void check_program_nodes(SA_tree_node* node);
      void check_global_def(SA_tree_node* node);
        sType get_global_type(SA_tree_node* node);
          semanticky_typ get_arg_def_type(SA_tree_node* node);
      void declare_argument_variable(psTree* localFrame, SA_tree_node* func_arg_def);
            semanticky_typ* create_def_arg_type_list(SA_tree_node* node, psData functionNode);
    
    void check_function(SA_tree_node* node);
    void check_function_call(psData function, SA_tree_node* expressionList, psTree localStack);
    
    semanticky_typ* check_function_body(SA_tree_node* node, psTree localStack);
      void check_code(SA_tree_node* node, psTree localStack);
        void check_line_of_code(SA_tree_node* node, psTree localStack);
          semanticky_typ* identifier_list_types(SA_tree_node* node, psTree localStack);
            semanticky_typ check_identifier_type(SA_tree_node* node, psTree localStack);
        
        void check_if_block(SA_tree_node* node, psTree localStack);
        void check_while_block(SA_tree_node* node, psTree localStack);
        void check_local_def(SA_tree_node* node, psTree* localStack);
       
        void check_assignment(SA_tree_node* node, psTree localStack);
    
    void typecast_to_type(semanticky_typ castTo, SA_tree_node* expression, psTree localStack);
    void check_var_definition(SA_tree_node* node, psData newNode);
    
    semanticky_typ get_semantic_type(key_word_value value);
    semanticky_typ get_arg_type(SA_tree_node* node);
    semanticky_typ* create_types_list(SA_tree_node* node);

    semanticky_typ* check_return_statement(SA_tree_node* node, semanticky_typ* castTo, psTree localStack);
    semanticky_typ* create_return_list(SA_tree_node* node);
    bool check_returns(semanticky_typ* declared, semanticky_typ* actual);    
      semanticky_typ* expression_list_types(SA_tree_node* node, semanticky_typ* castToList, psTree localStack);
    
    
    void check_main_function(SA_tree_node* node);
    
    semanticky_typ check_expression(SA_tree_node* node, psTree localStack);
    void check_condition(SA_tree_node* node, psTree localStack);

#endif	// SEMANTICKA_ANALYZA_H



