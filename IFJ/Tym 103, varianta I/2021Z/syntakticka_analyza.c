/** 
 * @file syntakticka_analyza.c
 * @brief implementace parseru. 
 * Funkce potřebné pro syntaktickou analýzu
 * @author Andrii Dovbush <xdovbu00@stud.fit.vutbr.cz>
 * @author Denis Kramár <xkrama06@stud.fit.vutbr.cz>
 */

#include "syntakticka_analyza.h"

LA_token* token;  // Current token;
SA_tree_node* add_node;  // globální ukazatel, pro nově přidávané uzly. Slouzi jen jako bool pro create_token, takze staci jedina promenna

void parser() {  // vytvori strom (Syntax) -> semanticka analyza -> generator kodu
	SA_tree code_tree;
	create_tree(&code_tree);  // N_start_node
	if (exit_code == SUCCESS) {  // volani syntakticke analyzy.
		semanticka_analyza(&code_tree);	  // volani semanticke analyzy.
	}
}

SA_tree_node* create_node(SA_tree_node* root, tree_node_type type) {  // vytvori synovsky uzel.
	add_node = sa_create_node();  // vytvori obecny uzel.
	if (add_node != NULL) {  // uzel uspesne vytvoren.
		add_node->parent = root;  // ukazatel na otcovsky uzel.
		add_node->type = type;  // typ neterminalu
	}
	return add_node;  // success == ukazatel na uzel. fail == NULL.
}

SA_tree_node* create_terminal_node(SA_tree_node* root) {  // definuje terminalni uzel.
	add_node = sa_create_term_node();  // vytvori terminalni uzel.
	if (add_node != NULL) {  // uzel uspesne vytvoren.
		add_node->parent = root;  // ukazatel na otcovsky uzel.
		add_node->token = token;  // naplni data terminalu.
	}
	return add_node;  // success == ukazatel na uzel. fail == NULL.
}

void create_tree(SA_tree* tree) {  // 1. S -> key_require + "ifj21" + N_program + N_main_start + EoF
	CREATE_NODE_PTR(root)
	root->type = N_start_node;
	GET_TOKEN  // Get "require" token
	if (TT == key_word && KWRD == kw_require) {  // kontrola spravnosti.
		MANAGE_TERM  // "require"
		if (TT == string && !strcmp(token->data.str, "ifj21")) {  // kontrola spravnosti.
			MANAGE_TERM  // "ifj21"
			MANAGE_NON_TERM(N_program, Program)
			MANAGE_NON_TERM(N_main_start, Main_start)
			GET_TOKEN  // EoF
			if (TT == EoF) {
				PUSH_TERM  // EoF
				tree->N_start_node = root;
				return;
			}
		}
	}
	exit_code = SYNTAX_ERR;
}

void Program(SA_tree_node* root) {
	if (TT != identif) {
		if (TT == key_word && KWRD == kw_global) {  // 2. N_program -> N_global_def + N_program
			MANAGE_NON_TERM(N_global_def, Global_def)
		} else if (TT == key_word && KWRD == kw_function) {  // 3. N_program -> N_function_def + N_program
			MANAGE_NON_TERM(N_function_def, Function_def)
		}
		MANAGE_NON_TERM(N_program, Program)
	}
}  // 4. N_program ->

void Global_def(SA_tree_node* root) {	  // 5. N_global_def -> key_global + identifier + definition + N_global_type
	MANAGE_TERM  // key_global. checked in Program()
	if (TT == identif) {
		MANAGE_TERM  // identifier
		if (TT == definition) {
			MANAGE_TERM  // ':'
			MANAGE_NON_TERM(N_global_type, Global_type)
		} else {
			exit_code = SYNTAX_ERR;
		}
	} else {
		exit_code = SYNTAX_ERR;
	}
}

void Global_type(SA_tree_node* root) {
	if (Arg_type() == true) {  // 6. N_global_type -> N_arg_type + N_type_def
		MANAGE_TERM  // key_string/key_integer/key_number
		MANAGE_NON_TERM(N_type_def, Type_def)
    return;
	}	else if (TT == key_word && KWRD == kw_function) {	  // 7. N_global_type -> key_function + '(' + N_def_arg_list + ')' + N_return_list
		MANAGE_TERM  // key_function
		if (TT == bracket_open) {
			MANAGE_TERM  // '('
			MANAGE_NON_TERM(N_def_arg_list, Types_list)
      if (TT == bracket_close) {
			  MANAGE_TERM  // ')'
			  MANAGE_NON_TERM(N_return_list, Return_list)
        return;
      }
		}
	}
  exit_code = SYNTAX_ERR;
}

void Type_def(SA_tree_node* root) {
	if (TT == assignment) {  // 8. N_type_def -> assignment + expression
		MANAGE_TERM  // assignment
		MANAGE_NON_TERM(N_expression, Expression)
	}
}  // 9. N_type_def ->

void Def_arg_list(SA_tree_node* root) {  // 10. N_def_arg_list -> N_arg_def + N_def_arg_list_inner
	if (TT != bracket_close) {
		MANAGE_NON_TERM(N_arg_def, Arg_def)
		MANAGE_NON_TERM(N_def_arg_list_inner, Def_arg_list_inner)
	}
}

void Def_arg_list_inner(SA_tree_node* root) {
	if (TT == comma) {  // 11. N_def_arg_list_inner -> comma + N_arg_def + N_def_arg_list_inner
		MANAGE_TERM  // ','
		MANAGE_NON_TERM(N_arg_def, Arg_def)
		MANAGE_NON_TERM(N_def_arg_list_inner, Def_arg_list_inner)
	}
}  // 12. N_def_arg_list_inner ->

void Arg_def(SA_tree_node* root) {
  if (TT == identif) {  // 13. N_arg_def -> identifier + definition + N_arg_type
    MANAGE_TERM  // identifier
    if (TT == definition) {
      MANAGE_TERM	  // ':'
      if (Arg_type() == true) {
        MANAGE_TERM  // key_string/key_integer/key_number
        return;
      }
    }
    exit_code = SYNTAX_ERR;
  }
}  // 14. N_arg_def ->

bool Arg_type() {  // 15./16./17. arg_type -> key_string/key_integer/key_number
	if ((TT == key_word) && (KWRD == kw_string	|| KWRD == kw_integer	|| KWRD == kw_number)) {
    return true;
	}
  return false;
}

void Function_def(SA_tree_node* root) {  // 18. N_function_def -> key_function + identifier + '(' + N_def_arg_list + ')' + N_return_list + N_code + key_end
  MANAGE_TERM	  // key_function. checked in Program()
  if (TT == identif) {
    MANAGE_TERM  // identifier 
    if (TT == bracket_open) {
      MANAGE_TERM  // '(' 
      MANAGE_NON_TERM(N_def_arg_list, Def_arg_list)
      MANAGE_TERM  // ')'
	  MANAGE_NON_TERM(N_return_list, Return_list)
      MANAGE_NON_TERM(N_code, Code)
      if (TT == key_word && KWRD == kw_end) {
        MANAGE_TERM  // key_end
        return;
      }
    }
	}
	exit_code = SYNTAX_ERR;
}

void Return_list(SA_tree_node* root) {
	if (TT == definition) {  //	19. return_list -> definition + types_list
		MANAGE_TERM	  // ':'
    MANAGE_NON_TERM(N_types_list, Types_list)
	}
}  //	20. return_list ->

void Types_list(SA_tree_node* root) {  // 21. types_list -> arg_type + types_list_inner
	if (Arg_type() == true) {
		MANAGE_TERM  // key_string/key_integer/key_number
    MANAGE_NON_TERM(N_types_list_inner, Types_list_inner)
	}
	else if (TT == bracket_close) {
		MANAGE_NON_TERM(N_types_list_inner, Types_list_inner)
	}
	else{
    exit_code = SYNTAX_ERR;
  }
}

void Types_list_inner(SA_tree_node* root) {
	if (TT == comma) {  // 22. N_types_list_inner -> comma + N_arg_type + N_types_list_inner
		MANAGE_TERM  // ','
		if (Arg_type() == true) {
			MANAGE_TERM  // key_string/key_integer/key_number
      MANAGE_NON_TERM(N_types_list_inner, Types_list_inner)
		} else {
      exit_code = SYNTAX_ERR;
    }
	}
}  // 23. N_types_list_inner -> 

void Code(SA_tree_node* root) {
	if (TT == identif) {  // 24. N_code -> N_line_of_code + N_code
    MANAGE_NON_TERM(N_line_of_code, Line_of_code)
	} else if (TT == key_word && KWRD == kw_if) {  // 25. N_code -> N_if_block + N_code
    MANAGE_NON_TERM(N_if_block, If_block)
	} else if (TT == key_word && KWRD == kw_while) {  // 26. N_code -> N_while_block + N_code
    MANAGE_NON_TERM(N_while_block, While_block)
	} else	if (TT == key_word && KWRD == kw_local) {  // 27. N_code -> N_local_def + N_code
    MANAGE_NON_TERM(N_local_def, Local_def)
	} else if (TT == key_word && KWRD == kw_return) {  // 28. code -> return_statement + code
    MANAGE_NON_TERM(N_return_statement, Return_statement)
	} else {  // 29. N_code ->
    return;
  }
  MANAGE_NON_TERM(N_code, Code)  // N_code
}

void Line_of_code(SA_tree_node* root) {  // 30. N_line_of_code -> identifier + N_identifier_contin
	if (TT == identif) {
		MANAGE_TERM  // identifier
		MANAGE_NON_TERM(N_identifier_contin, Identifier_contin)
	} else {
		exit_code = SYNTAX_ERR;
	}
}

void If_block(SA_tree_node* root) {  // 31. N_if_block -> key_if + N_condition + key_then + N_code + N_else_block + key_end
	if (TT == key_word && KWRD == kw_if) {
		MANAGE_TERM  // key_if
		MANAGE_NON_TERM(N_condition, Condition)
		if (TT == key_word && KWRD == kw_then) {
			MANAGE_TERM  // key_then
			MANAGE_NON_TERM(N_code, Code)
			MANAGE_NON_TERM(N_else_block, Else_block)
			if (TT == key_word && KWRD == kw_end) {
				MANAGE_TERM  // key_end
				return;
			}
		}
	}
	exit_code = SYNTAX_ERR;
}

void Else_block(SA_tree_node* root) {
 	if (TT == key_word && KWRD == kw_else) {  // 32. N_else_block -> key_else + N_code
		MANAGE_TERM  // key_else 
		MANAGE_NON_TERM(N_code, Code)
	}
}  // 33. N_else_block -> 

void While_block(SA_tree_node* root) {
	if (TT == key_word && KWRD == kw_while) {  // 34. N_while_block -> key_while + N_condition + key_do + N_code + key_end
		MANAGE_TERM	  // key_while
		MANAGE_NON_TERM(N_condition, Condition)
		if (TT == key_word && KWRD == kw_do) {
			MANAGE_TERM	  // key_do
			MANAGE_NON_TERM(N_code, Code)
			if (TT == key_word && KWRD == kw_end) {
				MANAGE_TERM	  // key_end
				return;
			}
		}
	}
	exit_code = SYNTAX_ERR;
}

void Local_def(SA_tree_node* root) {  // 35. N_local_def -> key_local + identifier + definition + N_arg_type + N_type_def
	if (TT == key_word && KWRD == kw_local) {
		MANAGE_TERM  // key_local
		if (TT == identif) {
			MANAGE_TERM	  // identifier
			if (TT == definition) {
				MANAGE_TERM	  // ':'
				if (Arg_type() == true) {	
					MANAGE_TERM  // key_string/key_integer/key_number
          MANAGE_NON_TERM(N_type_def, Type_def)
          return;
				}
			}
		}
	}
  exit_code = SYNTAX_ERR;
}

void Return_statement(SA_tree_node* root) {
	if (TT == key_word && KWRD == kw_return) {  // 36. N_return_statement -> key_return + N_expression_list 
		MANAGE_TERM  // key_return
	  if (TT == identif || TT == string || TT == double_value || TT == integer_value || (TT == key_word && TT == kw_nil)) {
			MANAGE_NON_TERM(N_expression_list, Expression_list)
		}
	} else {
		exit_code = SYNTAX_ERR;
	}
}

void Identifier_contin(SA_tree_node* root) {
	if (TT == bracket_open) {  // 37. N_identifier_contin -> '(' + N_expression_list + ')'
		MANAGE_TERM  // '('
		MANAGE_NON_TERM(N_expression_list, Expression_list)
		if (TT == bracket_close) {
			MANAGE_TERM  // ')'
		} else {
			exit_code = SYNTAX_ERR;
		}
	} else {  // 38. N_identifier_contin -> N_identifier_list_inner + assignment + N_expression_list
		MANAGE_NON_TERM(N_identifier_list_inner, Identifier_list_inner)
		if (TT == assignment) {
			MANAGE_TERM  // '='
			MANAGE_NON_TERM(N_expression_list, Expression_list)
		} else {
			exit_code = SYNTAX_ERR;
		}
	}
}

void Identifier_list_inner(SA_tree_node* root) {
	if (TT == comma) {  // 39. N_identifier_list_inner -> comma + identifier + N_identifier_list_inner
		MANAGE_TERM  // comma
		if (TT == identif) {
			MANAGE_TERM  // identifier
			MANAGE_NON_TERM(N_identifier_list_inner, Identifier_list_inner)
		} else {
			exit_code = SYNTAX_ERR;
		}
	}
}  // 40. N_identifier_list_inner ->

void Expression_list(SA_tree_node* root) {
	if (TT != bracket_close) {  // 41. N_expression_list -> N_expression + N_expression_list_inner
		MANAGE_NON_TERM(N_expression, Expression)
		MANAGE_NON_TERM(N_expression_list_inner, Expression_list_inner)
	}
}  // 42. N_expression_list ->

void Expression_list_inner(SA_tree_node* root) {
	if (TT == comma) {  // 43. N_expression_list_inner -> comma + N_expression + N_expression_list_inner
		MANAGE_TERM  // comma
		MANAGE_NON_TERM(N_expression, Expression)
		MANAGE_NON_TERM(N_expression_list_inner, Expression_list_inner)
	}
}  // 44. N_expression_list_inner ->

void Main_start(SA_tree_node* root) {  // 45. N_main_start -> key_main + '(' + ')'
	if (TT == identif && !strcmp(token->data.str, "main")) {
		MANAGE_TERM  // main
		if (TT == bracket_open) {
			MANAGE_TERM  // '('
			if (TT == bracket_close) {
				MANAGE_TERM  // ')'
				return;
			}
		}
	}
	exit_code = SYNTAX_ERR;
}

void Str_len_arg(SA_tree_node* root) {
	if (TT == string) {  // Str_len_arg -> string
		MANAGE_TERM  // identifier
	} else if (TT == identif) {  // Str_len_arg -> identifier
		MANAGE_TERM  // string 
		if (TT == bracket_open) {  // Str_len_arg -> identifier + '(' + N_expression + ')'
			MANAGE_TERM  // '('
      MANAGE_NON_TERM(N_expression, Expression)
			if (TT == bracket_close) {
				MANAGE_TERM  // ')'
			} else {
				exit_code = SYNTAX_ERR;
			}
		}
	} else {
	  exit_code = SYNTAX_ERR;
  }
}

void Condition(SA_tree_node* root) {  // Condition -> N_expression + less/less_equals/greater/greater_equals/not_equals/equals/ + expression
	 MANAGE_NON_TERM(N_expression, Expression)
	 if (TT == less ||	TT == less_eq  || TT == greater || TT == greater_eq || TT == not_eq || TT == eq) {
		 MANAGE_TERM	  // < / <= / > / >= / ~= / ==
		MANAGE_NON_TERM(N_expression, Expression)
	} else {
		exit_code = SYNTAX_ERR;
	}
}

//======================== EXPRESSION SOLVERS ============================

bool is_expression_node(SA_tree_node* node) {
	return node->type == N_expression;
}

bool is_i_token(LA_token* term_token) {
	return term_token->type == identif
		|| term_token->type == string
		|| term_token->type == double_value
		|| term_token->type == integer_value
		|| (term_token->type == key_word && term_token->data.key_word == kw_nil);
}

bool is_i_node(SA_tree_node* node) {
	if (node->type == N_terminal || node->type == N_function_call) {
		LA_token* term_token = node->token;
		return is_i_token(term_token);
	}
	return false;
}

bool is_bioperand_token(LA_token* operand) {
	return operand->type == plus
		|| operand->type == minus
		|| operand->type == times
		|| operand->type == division
		|| operand->type == whole_div
		|| operand->type == concat;
}

bool is_bioperand_node(SA_tree_node* node) {
	if (node->type == N_terminal) {
		LA_token* operand = node->token;
		return is_bioperand_token(operand);
	}
	return false;
}

bool is_str_len(SA_tree_node* node) {
	return node->type == N_terminal && node->token->type == str_len;
}

bool end_of_expression(LA_token* last, LA_token* next) {
	if (last) {
		if (next) {
			return (is_i_token(last) || last->type == bracket_close) && !is_bioperand_token(next);
		}
		return true;
	}
	return false;
}

char prec_table(LA_token* top, LA_token* next) {
	char precTable[7][7] = {
		//+- |*/..//| # | ( |  ) |  i |  $  |
		{ '>', '<', '<', '<', '>', '<', '>' }, // +-
		{ '>', '>', '<', '<', '>', '<', '>' }, // */..//
		{ '>', '>', ' ', '<', '>', '<', '>' }, // #
		{ '<', '<', '<', '<', '=', '<', ' ' }, // (
		{ '>', '>', ' ', ' ', '>', ' ', '>' }, // )
		{ '>', '>', ' ', ' ', '>', ' ', '>' }, // i
		{ '<', '<', '<', '<', ' ', '<', ' ' }  // $
	};

	unsigned x = 0, y = 0;
	if (top == 1) {  // nerozumim
		x = 6;
	} else if (top->type == plus || top->type == minus) {
		x = 0;
	} else if (top->type == times || top->type == division || top->type == concat ||  top->type == whole_div) {
		x = 1;
	} else if (top->type == str_len) {
		x = 2;
	} else if (top->type == bracket_open) {
		x = 3;
	} else if (top->type == bracket_close) {
		x = 4;
	} else if (is_i_token(top)) {
		x = 5;
	} else {
		return ' ';
	}

	if (!next) {
		y = 6;
	} else if (next->type == plus || next->type == minus) {
		y = 0;
	} else if (next->type == times ||
		next->type == division ||
		next->type == concat ||
		next->type == whole_div) {
		y = 1;
	} else if (next->type == str_len) {
		y = 2;
	} else if (next->type == bracket_open) {
		y = 3;
	} else if (next->type == bracket_close) {
		y = 4;
	} else if (is_i_token(next)) {
		y = 5;
	} else {
		y = 6;
	}

	return precTable[x][y];
}

void copy_top_nodes_from_stack_and_delete(n_vector_t* stack, n_vector_t* node_vec) {
	n_vector_node_t* last_node = stack->last;
	unsigned size = 0;
	while (last_node && !(last_node->node->type == N_terminal && !last_node->node->token)) { //while last_node != '<'
		last_node = last_node->prev;
		size++;
	}

	if (last_node) {
		node_vec->last = last_node;

		stack->last = last_node; //stack->last = '<'
		last_node = last_node->next; 
		last_node->prev->next = NULL;
		stack->size -= size;

		node_vec->first = last_node;
		node_vec->size = size;

		n_vec_pop_back(stack);
	} else {
		exit_code = SYNTAX_ERR;
	}
}

void set_parentity(SA_tree_node* node) {
	for (unsigned i = 0; i < node->nodes->size; i++) {
		SA_tree_node* child = n_vec_get(node->nodes, i);
		child->parent = node;
	}
}

void swap_top_rule_on_stack(n_vector_t* stack) {
	SA_tree_node* top = stack->last->node;
	SA_tree_node* new_expression_node = create_node(NULL, N_expression);
	if (new_expression_node) {
		copy_top_nodes_from_stack_and_delete(stack, new_expression_node->nodes);
		if (is_i_node(top)) {
			if (new_expression_node->nodes->size == 1) {
				SA_tree_node* first_node_of_rule = n_vec_get_first(new_expression_node->nodes);
				if (!is_i_node(first_node_of_rule)) {
					exit_code = SYNTAX_ERR;
				}
			} else if (new_expression_node->nodes->size == 2) {
				SA_tree_node* first_node_of_rule = n_vec_get(new_expression_node->nodes, 0);
				SA_tree_node* second_node_of_rule = n_vec_get(new_expression_node->nodes, 1);

				if (!is_expression_node(second_node_of_rule)
					|| !(first_node_of_rule->type == N_terminal && first_node_of_rule->token->type == str_len)) {
					exit_code = SYNTAX_ERR;
				}
			} else {
				exit_code = SYNTAX_ERR;
			}
		} else if (is_expression_node(top)) {
			if (new_expression_node->nodes->size == 2) {
				SA_tree_node* first_node_of_rule = n_vec_get(new_expression_node->nodes, 0);
				SA_tree_node* second_node_of_rule = n_vec_get(new_expression_node->nodes, 1);
				if (!is_str_len(first_node_of_rule)
					|| !is_expression_node(second_node_of_rule)) {
					exit_code = SYNTAX_ERR;
				}
			} else if (new_expression_node->nodes->size == 3) {
				SA_tree_node* first_node_of_rule = n_vec_get(new_expression_node->nodes, 0);
				SA_tree_node* second_node_of_rule = n_vec_get(new_expression_node->nodes, 1);
				SA_tree_node* third_node_of_rule = n_vec_get(new_expression_node->nodes, 2);
				if (!is_expression_node(first_node_of_rule)
					|| !is_bioperand_node(second_node_of_rule)
					|| !is_expression_node(third_node_of_rule)) {
					exit_code = SYNTAX_ERR;
				}
			} else {
				exit_code = SYNTAX_ERR;
			}
		} else if (top->type == N_terminal && top->token->type == bracket_close) {
			if (new_expression_node->nodes->size == 3) {
				LA_token* first_token_of_rule = n_vec_get(new_expression_node->nodes, 0)->token;
				SA_tree_node* second_node_of_rule = n_vec_get(new_expression_node->nodes, 1);
				LA_token* third_token_of_rule = n_vec_get(new_expression_node->nodes, 2)->token;
				if (first_token_of_rule->type != bracket_open ||
					!is_expression_node(second_node_of_rule) ||
					third_token_of_rule->type != bracket_close) {
					exit_code = SYNTAX_ERR;
				}
			} else {
				exit_code = SYNTAX_ERR;
			}
		} else {
			exit_code = SYNTAX_ERR;
		}

		set_parentity(new_expression_node);
	} else {
		exit_code = INTERNAL_ERR;
	}

	n_vec_push_back(stack, new_expression_node);
}

LA_token* get_top_terminal_token(n_vector_t* stack) {
	n_vector_node_t* top = stack->last;
	while (top && !((top->node->type == N_terminal || top->node->type == N_function_call) && top->node->token)) {
		top = top->prev;
	}
	if (top) {
		return top->node->token;
	}
	return NULL;
}

unsigned get_top_terminal_index(n_vector_t* stack) {
	n_vector_node_t* top = stack->last;
	LA_token* tokenFrom = top->node->token;
	unsigned index = stack->size - 1;
	while (top && !((top->node->type == N_terminal || top->node->type == N_function_call) && top->node->token)) {
		tokenFrom = top->node->token;
		index--;
		top = top->prev;
	}
	if (top) {
		return index;
	}
	return 0;
}

void Expression(SA_tree_node* root) {  // Rules for "expression" non-terminal
	n_vector_t* stack;
	n_vec_init(&stack);

	SA_tree_node* node = create_node(NULL, N_terminal);
	node->token = 1;
	n_vec_push_back(stack, node);

	LA_token* top = 1;
	LA_token* last = NULL;
	LA_token* next = token;
	while (!end_of_expression(last, next) || top != 1) {
		if (last && next) {
			if ((is_i_token(last) || last->type == bracket_close) && next->type == identif) {
				next = NULL;
			}
		}
		char table_res = prec_table(top, next);
		switch (table_res) {

		case '>': {
			swap_top_rule_on_stack(stack);	//
			if (exit_code != SUCCESS) {
				n_vec_sa_tree_nodes_free(stack);
				n_vec_free(stack);
				return;
			}
			break;
		}

		case '<': {
			SA_tree_node* node = create_node(NULL, N_terminal);
			n_vec_insert_after_id(stack, get_top_terminal_index(stack), node);

			if (TT == identif) {
				last = next;
				GET_TOKEN
				next = token;
				if (next->type == bracket_open) {
					node = create_node(NULL, N_function_call);
					node->token = last;
					Identifier_contin(node);
					if (exit_code == SUCCESS) {
						n_vec_push_back(stack, node);	// pushing func_call node
					} else {
						n_vec_sa_tree_nodes_free(stack);
						n_vec_free(stack);
						exit_code = SYNTAX_ERR;
						return;
					}
				} else {
					node = create_node(NULL, N_terminal);
					node->token = last;
					n_vec_push_back(stack, node);	// pushing identifier token node
				}
			} else {
				node = create_node(NULL, N_terminal);
				node->token = next;
				n_vec_push_back(stack, node);	// pushing next token
				last = next;
				GET_TOKEN
			}
			next = token;
			break;
		}

		case '=': {
			SA_tree_node* node = create_node(NULL, N_terminal);
			node->token = next;
			n_vec_push_back(stack, node);	// pushing next token

			last = next;
			GET_TOKEN
			next = token;
			break;
		}

		default:
			n_vec_sa_tree_nodes_free(stack);
			n_vec_free(stack);
			exit_code = SYNTAX_ERR;
			return;
		}

		top = get_top_terminal_token(stack);
	}

	n_vec_free(root->nodes);
	root->nodes = n_vec_get(stack,1)->nodes;

	n_vec_free(stack);
}