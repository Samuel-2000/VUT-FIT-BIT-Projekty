/** 
 * @file semanticka_analyza.c
 * @brief semanticka analýza
 * Funkce potřebné pro sémantickou analýzu
 * @author Andrii Dovbush <xdovbu00@stud.fit.vutbr.cz>
 * @author Denis Kramár <xkrama06@stud.fit.vutbr.cz>
 */

#include "semanticka_analyza.h"

psTree programTable;

semanticky_typ* needed_return;

unsigned sizeOfSemTypeArr(semanticky_typ* ary) {
	unsigned size = 0;
	while (*ary <= 2 && *ary >= 0) {
		size++;
		ary++;
	}
	return size;
}

psData initNodeData() {	// Allocates and initialize psData
	if (exit_code == SUCCESS) {
		psData nodeData = (psData)malloc(sizeof(struct sData));
		if (nodeData) {
			nodeData->defined = false;	// Not defined
			nodeData->params = 0;
			nodeData->localFrame = NULL; // Dont have local variables
			nodeData->type = -1;		 // Undefined type
			nodeData->args = NULL;		 // Dont have arguments
			nodeData->returns = NULL;	 // Dont have return values
			return nodeData;
		} else {
			exit_code = INTERNAL_ERR;	 // Internal compiler error: Mem. allocation fail
		}
	}
	return NULL;
}

void insert_to_smthng_node(SA_tree_node* node, tree_node_type type) {
	if (exit_code != SUCCESS) {
		SA_tree_node* newNode = sa_create_node();
		if (newNode) {
			newNode->type = type;	// Setting type of new node
			newNode->parent = node;	// Inserting new node after the node.
			//NODE VECTOR SWAP
			n_vector_t* newNodeVec = newNode->nodes;
			newNode->nodes = node->nodes;
			node->nodes = newNodeVec;
		} else {
			exit_code = INTERNAL_ERR;  // Internal compiler error: Mem. allocation fail
		}
	}
}

void insert_to_number_node(SA_tree_node* node) {
	insert_to_smthng_node(node, N_to_number);	// Inserting to_number node
}

void insert_to_integer_node(SA_tree_node* node) {
	insert_to_smthng_node(node, N_to_integer);	// Inserting to_integer node
}

semanticky_typ get_sem_type_from_s_type(sType type) {
	if (type == string_var) {
		return string_val;
	} else if (type == integer_var) {
		return integer_val;
	} else if (type == number_var) {
		return number_val;
	}
	exit_code = INTERNAL_ERR;
	return -1;
}

semanticky_typ get_sem_type_from_token(LA_token* token) {
	if (token->data.key_word == kw_string) {
		return string_val;
	} else if (token->data.key_word == kw_integer) {
		return integer_val;
	} else if (token->data.key_word == kw_number) {
		return number_val;
	}
	exit_code = INTERNAL_ERR;
	return -1;
}

sType get_s_type_from_token(LA_token* token) {
	if (token->data.key_word == kw_integer) return integer_var;
	else if (token->data.key_word == kw_number) return number_var;
	else if (token->data.key_word == kw_string) return string_var;
	else if (token->data.key_word == kw_function) return func;
	exit_code = INTERNAL_ERR;
	return -1;
}

semanticky_typ get_local_type(SA_tree_node* node) {
	LA_token* type = node->token;
	return get_sem_type_from_token(type);
}

unsigned count_inner_nodes(SA_tree_node* list_inner) {
	unsigned size = 0;
	while (list_inner->nodes->size) {
		size++;
		//	*list_inner -> comma + * + *list_inner
		list_inner = n_vec_get(list_inner->nodes, 2);
	}
	return size;
}

void createWriteFunction() {
	psData writeFunc = initNodeData();
	if (writeFunc) {
		writeFunc->args = NULL;
		writeFunc->defined = true;
		writeFunc->localFrame = NULL;
		writeFunc->params = 0;
		writeFunc->returns = NULL;
		writeFunc->type = func;
		symTabInsert(&programTable, "write", writeFunc);
	}
}

void createReadSFunction() {
	psData writeFunc = initNodeData();
	if (writeFunc) {
		writeFunc->args = NULL;
		writeFunc->defined = true;
		writeFunc->localFrame = NULL;
		writeFunc->params = 0;
		writeFunc->type = func;
		semanticky_typ* returns = (semanticky_typ*)malloc(sizeof(semanticky_typ));
		if (returns) {
			returns[0] = string_val;
			writeFunc->returns = returns;
			symTabInsert(&programTable, "reads", writeFunc);
			return;
		}
		exit_code = INTERNAL_ERR;
	}
}

void createReadIFunction() {
	psData writeFunc = initNodeData();
	if (writeFunc) {
		writeFunc->args = NULL;
		writeFunc->defined = true;
		writeFunc->localFrame = NULL;
		writeFunc->params = 0;
		writeFunc->type = func;
		semanticky_typ* returns = (semanticky_typ*)malloc(sizeof(semanticky_typ));
		if (returns) {
			returns[0] = integer_val;
			writeFunc->returns = returns;
			symTabInsert(&programTable, "readi", writeFunc);
			return;
		}
		exit_code = INTERNAL_ERR;
	}
}

void createReadNFunction() {
	psData writeFunc = initNodeData();
	if (writeFunc) {
		writeFunc->args = NULL;
		writeFunc->defined = true;
		writeFunc->localFrame = NULL;
		writeFunc->params = 0;
		writeFunc->type = func;
		semanticky_typ* returns = (semanticky_typ*)malloc(sizeof(semanticky_typ));
		if (returns) {
			returns[0] = number_val;
			writeFunc->returns = returns;
			symTabInsert(&programTable, "readn", writeFunc);
			return;
		}
		exit_code = INTERNAL_ERR;
	}
}

void createToIntegerFunction() {
	psData writeFunc = initNodeData();
	if (writeFunc) {
		semanticky_typ* args = (semanticky_typ*)malloc(sizeof(semanticky_typ));
		if (args) {
			args[0] = number_val;
			writeFunc->args = args;
			writeFunc->defined = true;
			writeFunc->localFrame = NULL;
			writeFunc->type = func;
			writeFunc->params = 0;
			semanticky_typ* returns = (semanticky_typ*)malloc(sizeof(semanticky_typ));
			if (returns) {
				returns[0] = integer_val;
				writeFunc->returns = returns;
				symTabInsert(&programTable, "tointeger", writeFunc);
				return;
			}
		}
		exit_code = INTERNAL_ERR;
	}
}

void createSubstrFunction() {
	psData writeFunc = initNodeData();
	if (writeFunc) {
		semanticky_typ* args = (semanticky_typ*)malloc(sizeof(semanticky_typ)*3);
		if (args) {
			args[0] = string_val;
			args[1] = number_val;
			args[2] = number_val;
			writeFunc->args = args;
			writeFunc->defined = true;
			writeFunc->localFrame = NULL;
			writeFunc->type = func;
			writeFunc->params = 0;
			semanticky_typ* returns = (semanticky_typ*)malloc(sizeof(semanticky_typ));
			if (returns) {
				returns[0] = string_val;
				writeFunc->returns = returns;
				symTabInsert(&programTable, "substr", writeFunc);
				return;
			}
		}
		exit_code = INTERNAL_ERR;
	}
}

void createOrdFunction() {
	psData writeFunc = initNodeData();
	if (writeFunc) {
		semanticky_typ* args = (semanticky_typ*)malloc(sizeof(semanticky_typ) * 2);
		if (args) {
			args[0] = string_val;
			args[1] = integer_val;
			writeFunc->args = args;
			writeFunc->defined = true;
			writeFunc->localFrame = NULL;
			writeFunc->type = func;
			writeFunc->params = 0;
			semanticky_typ* returns = (semanticky_typ*)malloc(sizeof(semanticky_typ));
			if (returns) {
				returns[0] = integer_val;
				writeFunc->returns = returns;
				symTabInsert(&programTable, "ord", writeFunc);
				return;
			}
		}
		exit_code = INTERNAL_ERR;
	}
}

void createChrFunction() {
	psData writeFunc = initNodeData();
	if (writeFunc) {
		semanticky_typ* args = (semanticky_typ*)malloc(sizeof(semanticky_typ));
		if (args) {
			args[0] = integer_val;
			writeFunc->args = args;
			writeFunc->defined = true;
			writeFunc->localFrame = NULL;
			writeFunc->type = func;
			writeFunc->params = 0;
			semanticky_typ* returns = (semanticky_typ*)malloc(sizeof(semanticky_typ));
			if (returns) {
				returns[0] = string_val;
				writeFunc->returns = returns;
				symTabInsert(&programTable, "chr", writeFunc);
				return;
			}
		}
		exit_code = INTERNAL_ERR;
	}
}

void setBasicFunctions() {
	createWriteFunction();
	createReadSFunction();
	createReadIFunction();
	createReadNFunction();
	createToIntegerFunction();
	createSubstrFunction();
	createOrdFunction();
	createChrFunction();
}

//====================================================================================================================

int semanticka_analyza(SA_tree* tree)  { // Main semantic analyzis function
	symTabInit(&programTable);			 // Table for global declarations init
	setBasicFunctions();
	check_program(tree->N_start_node);   // Check semantic dependencies
	return exit_code;
}

void check_program(SA_tree_node* node) {
	if (exit_code == SUCCESS) {
		for (unsigned i = 0; i < node->nodes->size; i++) {	// check each child node for starting node.
			check_program_nodes(n_vec_get(node->nodes, i));
		}
	}
}

void check_program_nodes(SA_tree_node* node) {
	if (exit_code == SUCCESS) {
		if (node->type == N_program) { 
			check_program(node);
		} else if (node->type == N_global_def) { 
			check_global_def(node);
		} else if (node->type == N_function_def) {
			check_function(node);
		}
	}
}

sType get_global_type(SA_tree_node* node) {
	LA_token* type = n_vec_get_first(node->nodes)->token;
	return get_s_type_from_token(type);
}

semanticky_typ get_arg_def_type(SA_tree_node* node) {
	if (exit_code == SUCCESS) {
		SA_tree_node* arg_type = n_vec_get(node->nodes, 2);
		LA_token* type = arg_type->token; // arg_def -> identifier + definition + arg_type
		return get_sem_type_from_token(type); // Get arg_type -> key_word 
	}
	return -1;
}

void declare_argument_variable(psTree* localFrame, SA_tree_node* func_arg_def) {
	if (exit_code == SUCCESS) {
		
		psData argumentNode = initNodeData();	// Creating new psData for table of declarations.
		if (argumentNode) {
			LA_token* type = n_vec_get(func_arg_def->nodes, 2)->token; // arg_def -> identifier + definition + arg_type
			LA_token* identifier = n_vec_get_first(func_arg_def->nodes)->token; // arg_def -> identifier + definition + arg_type
			argumentNode->type = get_s_type_from_token(type);	// get semantic_type from type token
			argumentNode->defined = true;
			if (exit_code == SUCCESS) { // if type was successfuly read
				symTabInsert(localFrame, identifier->data.str, argumentNode); //Inserting to table
			}
		}
	}
}

semanticky_typ* create_def_arg_type_list(SA_tree_node* node, psData functionNode) {
	 if (node->nodes->size) { //	def_arg_list ->arg_def + def_arg_list_inner
		SA_tree_node* def_arg_list_inner = n_vec_get(node->nodes, 1); // def_arg_list ->arg_def + def_arg_list_inner
		
		unsigned size = 1 + count_inner_nodes(def_arg_list_inner);	// calculating number of arg_defs
		
		
		semanticky_typ* args_types = (semanticky_typ*)malloc(sizeof(semanticky_typ) * size); // Memmory alloc for argumen types list of function node

		if (args_types) {

			args_types[0] = get_arg_def_type(n_vec_get_first(node->nodes));

			declare_argument_variable(&functionNode->localFrame, n_vec_get_first(node->nodes));	// declare local variadles to local Frame

			unsigned i = 1;
			while (def_arg_list_inner->nodes->size) {
				SA_tree_node* arg_type_node = n_vec_get(def_arg_list_inner->nodes, 1); //  def_arg_list->comma + arg_def + types_list_inner
				args_types[i] = get_arg_def_type(arg_type_node);

				declare_argument_variable(&functionNode->localFrame, arg_type_node);	// declare argument to localFrame table

				def_arg_list_inner = n_vec_get(def_arg_list_inner->nodes, 2);
				i++;
			}
			return args_types;
		} else {
			exit_code = INTERNAL_ERR; // Memmory allocate fail
		}
	}
	 return NULL;
}

void function_node_fill_psData(psData newNode, SA_tree_node* globalType) {
	// global_type->key_function + left_bracket + def_arg_list + right_bracket + return_list
	SA_tree_node* defArgList = n_vec_get(globalType->nodes, 2);
	SA_tree_node* returnList = n_vec_get(globalType->nodes, 4);

	newNode->args = create_types_list(defArgList);
	newNode->returns = create_return_list(returnList);

	if (exit_code != SUCCESS) {
		free(newNode);
	}
}

void variable_node_fill_psData(psData newNode, SA_tree_node* globalType) {
	// if type_def node has nodes (= defined)
	if (n_vec_get(globalType->nodes, 1)->nodes->size) {
		check_var_definition(globalType, newNode);
		//if check_var_definition has found the mistakes
		if (exit_code != SUCCESS) {
			free(newNode);
		}
	}
}

void check_global_def(SA_tree_node* node) {
	if (exit_code == SUCCESS) {
		LA_token* identifier = n_vec_get(node->nodes, 1)->token;
		
		if (!symTabSearch(&programTable, identifier->data.str)) { // If identifier is not declared

			psData newNode = initNodeData();
			
			SA_tree_node* globalType = n_vec_get(node->nodes, 3); // Looking for definition and type

			newNode->type = get_global_type(globalType); // Setting type (string_var, integer_var, number_var, function)
			
			if (globalType->nodes->size > 2) { // if it is function -> check function declaration
				function_node_fill_psData(newNode, globalType);
			} else { // if it is variable -> 
				variable_node_fill_psData(newNode, globalType);
			}

			if (exit_code == SUCCESS) {
				//Inserting 
				symTabInsert(&programTable, identifier->data.str, newNode);
			}
		} else {
			exit_code = SEM_UNDEF_ERR; // Identifier was declared previously
		}
	}
}

void typecast_checked_expression(semanticky_typ castTo, semanticky_typ castFrom, SA_tree_node* expression) {
	if (castTo == integer_val) {

		if (castFrom == integer_val || castFrom == integer_val_one_of) { // If definitive expression value type is integer
			return;
		} else if (castFrom == number_val_one_of) { // If definitive expression value type is number
			insert_to_integer_node(expression);
			return;
		}
	} else if (castTo == number_val) {
		if (castFrom == integer_val || castFrom == integer_val_one_of) { // If definitive expression value type is integer
			insert_to_number_node(expression);
			return;
		} else if (castFrom == number_val) { // If definitive expression value type is number
			return;
		}
	} else if (castTo == string_val && (castFrom == string_val || castFrom == string_val_one_of)) {
		return;
	}
	exit_code = SEM_ASS_TYPE_ERR; //sémantická chyba v příkazu přiřazení (typová nekompatibilita)
}

void typecast_to_type(semanticky_typ castTo, SA_tree_node* expression, psTree localStack) {
	semanticky_typ exprType = check_expression(expression,localStack);
	if (exit_code == SUCCESS) {
		typecast_checked_expression(castTo, exprType, expression);
	}
}

void check_var_definition(SA_tree_node* global_type_node, psData newNode) {
	if (exit_code == SUCCESS) {
		LA_token* type = n_vec_get(global_type_node->nodes, 0)->token; // global_type -> key_string/key_integer/key_number + type_def
		SA_tree_node* type_def = n_vec_get(global_type_node->nodes, 1); // global_type -> key_string/key_integer/key_number + type_def

		if (type_def->nodes->size) {
			newNode->defined = true;
			SA_tree_node* definition_expression = n_vec_get(type_def->nodes, 1);
			
			semanticky_typ typ = get_sem_type_from_token(type);
			typecast_to_type(typ, definition_expression, NULL); // Typecasting and expression check
		}
	}
	
}

void check_function(SA_tree_node* node) {
	// function_def->key_function + identifier + left_bracket + def_arg_list + right_bracket + return_list + function_body + key_end
	if (exit_code == SUCCESS) {
		LA_token* identifier = n_vec_get(node->nodes, 1)->token;
		SA_tree_node* defArgList = n_vec_get(node->nodes, 3);
		SA_tree_node* returnList = n_vec_get(node->nodes, 5);
		SA_tree_node* functionBody = n_vec_get(node->nodes, 6);

		//Creating function node for symbol table
		psData newNode = initNodeData();
		newNode->type = func;

		// Creating and adding to symbol table list node of function arg types
		newNode->args = create_def_arg_type_list(defArgList, newNode);
		// Creating and adding to symbol table list node of function return types
		newNode->returns = create_return_list(returnList);

		// If identifier is not declared
		if (!symTabSearch(&programTable, identifier->data.str)) {
			//Inserting 'function' node
			symTabInsert(&programTable, identifier->data.str, newNode);
		} else { 
			// Get function declaration from table
			psData foundFunc = symTabSearch(&programTable, identifier->data.str);
			// If argument list and return list is different from function definition
			if (!check_returns(foundFunc->args, newNode->args) || !check_returns(foundFunc->returns, newNode->returns)) {
				exit_code = SEM_FUN_PARAM_ERR;
				// Free newNode
				symTabDispose(&newNode->localFrame);
				free(newNode);
				return;
			}
			// Free newNode
			symTabDispose(&newNode->localFrame);
			free(newNode);
		}

		psData function = symTabSearch(&programTable, identifier->data.str);

		// GENERATE_CODE(gen_fun_beginning, identifier->data.str)

		function->defined = true;
		
		// Create a list of declared return types
		semanticky_typ* declared_returns = create_return_list(returnList);
		needed_return = declared_returns;
		// If create_return_list ran successfully 
		if (exit_code == SUCCESS) {
			check_code(functionBody, function->localFrame);
		}
		free(declared_returns);
	}
}

semanticky_typ* create_return_list(SA_tree_node* node) {
	//if return_list is not empty
	if (node->nodes->size > 0) {
		SA_tree_node* types_list = n_vec_get(node->nodes, 1); // return_list-> definition + types_list
		return create_types_list(types_list);
	}
	return NULL;
}

semanticky_typ* create_types_list(SA_tree_node* node) {
	// types_list -> arg_type + types_list_inner
	SA_tree_node* types_list_inner = n_vec_get(node->nodes, 1);
	if (types_list_inner) {
		// While there are return types
		unsigned size = 1 + count_inner_nodes(types_list_inner);

		semanticky_typ* declared_return = (semanticky_typ*)malloc(sizeof(semanticky_typ) * size);
		if (declared_return) {

			declared_return[0] = get_local_type(n_vec_get_first(node->nodes));

			unsigned i = 1;
			types_list_inner = n_vec_get(node->nodes, 1);

			// While there are return types
			while (types_list_inner->nodes->size) {
				SA_tree_node* arg_type_node = n_vec_get(types_list_inner->nodes, 1);// types_list_inner->comma + arg_type + types_list_inner
				declared_return[i] = get_local_type(arg_type_node);

				types_list_inner = n_vec_get(types_list_inner->nodes, 2);
				i++;
			}
			return declared_return;
		} else {
			exit_code = INTERNAL_ERR; // Memmoty allocate error
			return NULL;
		}
	}
	return NULL;
}

bool check_returns(semanticky_typ* declared, semanticky_typ* actual) { // compares two list of semanticky_typ variables
	while (declared && actual) {
		if (!((*declared == string_val ||
			 *declared == integer_val || 
			 *declared == number_val) || 
			(*actual == string_val ||
			*actual == integer_val ||
			*actual == number_val))) break;
		// If return types are different
		if (*declared != *actual) {
			// sémantická chyba v programu – špatný počet/typ parametrů
			// či návratových hodnot u volání funkce či návratu z funkce.
			exit_code = SEM_FUN_PARAM_ERR;
			return false;
		}
		declared++;
		actual++;
	}
	if (!declared || !actual) {
		if (!declared && !actual) return true;
		return false;
	} else if ((*declared == string_val ||
		*declared == integer_val ||
		*declared == number_val) && !(*actual == string_val ||
			*actual == integer_val ||
			*actual == number_val)) {
		// sémantická chyba v programu – špatný počet/typ parametrů
		// či návratových hodnot u volání funkce či návratu z funkce.
		exit_code = SEM_FUN_PARAM_ERR;
		return false;
	}
	return true;
}

bool check_function_args_call(semanticky_typ* declared, semanticky_typ* actual) { // compares two list of semanticky_typ variables
	while (declared && actual) {
		if (!((*declared == string_val ||
			*declared == integer_val ||
			*declared == number_val) ||
			(*actual == string_val ||
				*actual == integer_val ||
				*actual == number_val))) break;
		// If return types are different
		if (*declared != *actual) {
			// sémantická chyba v programu – špatný počet/typ parametrů
			// či návratových hodnot u volání funkce či návratu z funkce.
			exit_code = SEM_FUN_PARAM_ERR;
			return false;
		}
		declared++;
		actual++;
	}
	if (!declared || !actual) {
		if (!declared && !actual) return true;
		exit_code = SEM_FUN_PARAM_ERR;
		return false;
	} else if ((*declared == string_val ||
		*declared == integer_val ||
		*declared == number_val) || (*actual == string_val ||
			*actual == integer_val ||
			*actual == number_val)) {
		// sémantická chyba v programu – špatný počet/typ parametrů
		// či návratových hodnot u volání funkce či návratu z funkce.
		exit_code = SEM_FUN_PARAM_ERR;
		return false;
	}
	return true;
}

semanticky_typ* check_return_statement(SA_tree_node* node, semanticky_typ* castTo, psTree localStack) {
	if (exit_code == SUCCESS) {

		// return_statement->key_return + expression_list
		if (node->nodes->size > 1) {
			semanticky_typ* returns = expression_list_types(n_vec_get(node->nodes, 1), castTo, localStack);
			check_function_args_call(needed_return, returns);
		}
		// return_statement ->
	}
	return NULL;
}

void check_code(SA_tree_node* node, psTree localStack) {
	if (exit_code == SUCCESS) {
		if (node->nodes->size) {
			// code->if_block + code
			// code->line_of_code + code
			// code->local_def + code
			// code->while_block + code
			SA_tree_node* firstNode = n_vec_get_first(node->nodes);
			if (firstNode->type == N_if_block) {
				check_if_block(firstNode, localStack);
			} else if (firstNode->type == N_line_of_code) {
				check_line_of_code(firstNode, localStack);
			} else if (firstNode->type == N_local_def) {
				check_local_def(firstNode, &localStack);
			} else if (firstNode->type == N_while_block) {
				check_while_block(firstNode, localStack);
			} else if (firstNode->type == N_return_statement) {
				check_return_statement(firstNode,NULL, localStack);
			}
			if (exit_code == SUCCESS) {
				check_code(n_vec_get(node->nodes, 1), localStack);
			}
		}
	}
}

void check_if_block(SA_tree_node* node, psTree localStack) {
	// if_block -> key_if + condition + key_then + code + else_block + key_end
	if (exit_code == SUCCESS) {
		SA_tree_node* conditionNode = n_vec_get(node->nodes, 1);
		SA_tree_node* codeNode = n_vec_get(node->nodes, 3);
		SA_tree_node* else_block = n_vec_get(node->nodes, 4);
		check_condition(conditionNode, localStack);
		check_code(codeNode, localStack);
		if (else_block->nodes->size) {
			// else_block -> key_else + code
			check_code(n_vec_get(else_block->nodes, 1), localStack);
		}
	}
}

void check_while_block(SA_tree_node* node, psTree localStack) {
	// while_block -> key_while + condition + key_do + code + key_end
	if (exit_code == SUCCESS) {
		SA_tree_node* conditionNode = n_vec_get(node->nodes, 1);
		SA_tree_node* codeNode = n_vec_get(node->nodes, 3);
		check_condition(conditionNode, localStack);
		check_code(codeNode, localStack);
	}
}

void check_local_def(SA_tree_node* node, psTree* localStack) {
	
	if (exit_code == SUCCESS) {
		LA_token* identifier = n_vec_get(node->nodes, 1)->token; // local_def -> key_local + identifier + definition + arg_type + type_def

		// If identifier is not declared
		if (!symTabSearch(localStack, identifier->data.str)) {

			psData newNode = initNodeData();

			LA_token* type = n_vec_get(node->nodes, 3)->token;

			newNode->type = get_s_type_from_token(type);

			SA_tree_node* type_def = n_vec_get(node->nodes, 4);
			if (type_def->nodes->size) {
				newNode->defined = true;

				SA_tree_node* definition_expression = n_vec_get(type_def->nodes, 1);
				// Typecasting and expression check
				semanticky_typ var_type = get_sem_type_from_token(type);

 				typecast_to_type(var_type, definition_expression, *localStack);
			}
			//Inserting 
			symTabInsert(localStack, identifier->data.str, newNode);
		} else {
			exit_code = SEM_UNDEF_ERR; // Declared twice;
		}
	}
}

void check_line_of_code(SA_tree_node* node, psTree localStack) {
	// line_of_code->identifier + identifier_contin
	// identifier_contin->identifier_list_inner + assignment + expression_list
	// identifier_contin->left_bracket + expression_list + right_bracket
	// identifier_list_inner ->
	// identifier_list_inner->comma + identifier + identifier_list_inner

	if (exit_code == SUCCESS) {
		LA_token* identifier = n_vec_get_first(node->nodes)->token;
		SA_tree_node* identContin = n_vec_get(node->nodes, 1);
		//If it is function call
		SA_tree_node* identContin_first_node = n_vec_get_first(identContin->nodes);

		if (identContin_first_node->type != N_identifier_list_inner) {
			SA_tree_node* expression_list = n_vec_get(identContin->nodes, 1);
			psData function = symTabSearch(&programTable, identifier->data.str);
			if (strcmp(identifier->data.str, "write") == 0) {
			} else {
				//if function is defined
				if (function) {
					check_function_call(function, expression_list, localStack);
				}
				//if function is not defined/declared
				else {
					exit_code = SEM_UNDEF_ERR;
					return;
				}
			}
		}
		//If it is assignment
		else {
			check_assignment(node, localStack);
		}

	}
}

void check_condition(SA_tree_node* node, psTree localStack) {
	// condition -> expression + operator + expression
	if (exit_code == SUCCESS) {
		SA_tree_node* express1 = n_vec_get(node->nodes, 0);
		LA_token* operand = n_vec_get(node->nodes, 1)->token;
		SA_tree_node* express2 = n_vec_get(node->nodes, 2);
		semanticky_typ exp1typ = check_expression(express1, localStack);
		semanticky_typ exp2typ = check_expression(express2, localStack);
		if (exit_code == SUCCESS) {
			if (exp1typ == nil_val || exp2typ == nil_val) {
				if (operand->type != eq && operand->type != not_eq) {
					exit_code = SEM_EXPR_TYPE_ERR;
					return;
				}
			} else {
				if (exp1typ == integer_val || exp1typ == integer_val_one_of) {
					if (exp2typ == number_val || exp2typ == number_val_one_of) {
						insert_to_number_node(express1);
					} else if (exp2typ == string_val || exp2typ == string_val_one_of) {
						exit_code = SEM_EXPR_TYPE_ERR;
						return;
					}
				} else if (exp1typ == number_val || exp1typ == number_val_one_of) {
					if (exp2typ == integer_val || exp2typ == integer_val_one_of) {
						insert_to_number_node(express2);
					} else if (exp2typ == string_val || exp2typ == string_val_one_of) {
						exit_code = SEM_EXPR_TYPE_ERR;
						return;
					}
				} else if (exp1typ == string_val || exp1typ == string_val_one_of) {
					if (operand->type == eq || operand->type == not_eq) {
						if (exp2typ != string_val && exp2typ != string_val_one_of) {
							exit_code = SEM_EXPR_TYPE_ERR;
							return;
						}
					} else {
						exit_code = SEM_EXPR_TYPE_ERR;
						return;
					}
				}
			}
		}
	}
}

void check_function_call(psData function, SA_tree_node* expressionList, psTree localStack) {
	if (exit_code == SUCCESS) {
		semanticky_typ* expressionList_types = expression_list_types(expressionList, function->args, localStack);
		if (exit_code == SUCCESS) {
			check_function_args_call(function->args, expressionList_types);
		} else {
			if (exit_code == SEM_ASS_TYPE_ERR) {
				exit_code = SEM_FUN_PARAM_ERR;
			}
		}
		free(expressionList_types);
	}
}

void check_assignment(SA_tree_node* node, psTree localStack) {
	// line_of_code->identifier + identifier_contin
	// identifier_contin->identifier_list_inner + assignment + expression_list
	// identifier_contin->left_bracket + expression_list + right_bracket
	// identifier_list_inner ->
	// identifier_list_inner->comma + identifier + identifier_list_inner
	if (exit_code == SUCCESS) {
		SA_tree_node* identifier_contin = n_vec_get(node->nodes, 1);
		SA_tree_node* expressionList = n_vec_get(identifier_contin->nodes, 2);
		semanticky_typ* identifier_types = identifier_list_types(node, localStack);
		if (exit_code == SUCCESS) {
			semanticky_typ* expressionList_types = expression_list_types(expressionList, identifier_types, localStack);
			check_returns(identifier_types, expressionList_types);
			if (exit_code == SEM_FUN_PARAM_ERR) {
				exit_code = SEM_EXPR_TYPE_ERR;
			}
			//free(expressionList_types);
		}
		free(identifier_types);
	}
}

psData findPsData(LA_token* identifier_token, psTree localStack) {
	if (exit_code == SUCCESS) {
		psData identifierType = symTabSearch(&localStack, identifier_token->data.str);
		if (!identifierType) {
			identifierType = symTabSearch(&programTable, identifier_token->data.str);
			if (!identifierType) {
				exit_code = SEM_UNDEF_ERR;
			} else {
				return identifierType;
			}
		} else {
			return identifierType;
		}
	}
	return NULL;
}

semanticky_typ* expression_list_types(SA_tree_node* node, semanticky_typ* castToList, psTree localStack) {
	// expression_list ->
	// expression_list->expression + expression_list_inner
	// expression_list _inner ->
	// expression_list _inner->comma + expression + expression_list _inner
	if (exit_code == SUCCESS) {
		//	expression_list->expression + expression_list_inner
		if (node->nodes->size) {
			// expression_list->expression + expression_list_inner
			SA_tree_node* expression_list_inner = n_vec_get(node->nodes, 1);

			unsigned size = 1 + count_inner_nodes(expression_list_inner);

			// Memmory alloc for expression types list of function node
			semanticky_typ* expression_types = (semanticky_typ*)malloc(sizeof(semanticky_typ) * size);
			if (expression_types) {
				unsigned i = 1;

				SA_tree_node* expression_node = n_vec_get(node->nodes, 0);
				semanticky_typ type = check_expression(expression_node, localStack);
				if (type == integer_val || type == string_val || type == number_val || type == nil_val) {
					if (castToList) {
						typecast_checked_expression(castToList[0], type, n_vec_get(node->nodes, 0));
						if (exit_code == SUCCESS) {
							expression_types[0] = castToList[0];
						}
					} else {
						expression_types[0] = type;
					}
				} else if (type == -1) {
					free(expression_types);
					return NULL;
				}
				//FUNCTION CALL
				else {
					SA_tree_node* function_call_node = n_vec_get_first(expression_node->nodes);
					psData function = findPsData(function_call_node->token, localStack);
					if (function) {
						unsigned addSize = sizeOfSemTypeArr(function->returns);
						semanticky_typ* new_expression_types = (semanticky_typ*)realloc(expression_types, sizeof(semanticky_typ) * (size + addSize - 1));
						if (new_expression_types) {
							expression_types = new_expression_types;
							for (unsigned j = 0; j < addSize; j++) {
								expression_types[j] = function->returns[j];
							}
						}
						i = addSize;
					}
				}


				expression_list_inner = n_vec_get(node->nodes, 1);

				// While there are expressions
				while (expression_list_inner->nodes->size) {
					expression_node = n_vec_get(expression_list_inner->nodes, 1);
					semanticky_typ type = check_expression(expression_node, localStack);
					if (type == integer_val || type == string_val || type == number_val || type == nil_val) {
						if (castToList) {
							typecast_checked_expression(castToList[i], type, n_vec_get(node->nodes, 0));
							if (exit_code == SUCCESS) {
								expression_types[i] = castToList[i];
							}
						} else {
							expression_types[i] = type;
						}
					} else if (type == -1) {
						free(expression_types);
						return NULL;
					} else {
						SA_tree_node* function_call_node = n_vec_get_first(expression_node->nodes);
						psData function = findPsData(function_call_node->token, localStack);
						if (function) {
							unsigned addSize = sizeOfSemTypeArr(function->returns);
							semanticky_typ* new_expression_types = (semanticky_typ*)realloc(expression_types, sizeof(semanticky_typ) * (size + addSize - 1));
							if (new_expression_types) {
								expression_types = new_expression_types;
								for (unsigned j = 0; j < addSize; j++) {
									expression_types[j + i] = function->returns[j];
								}
							}
							i += addSize - 1;
						}
					}

					expression_list_inner = n_vec_get(expression_list_inner->nodes, 2);

					i++;
				}
				return expression_types;
			} else {
				// Memmory allocate error
				exit_code = INTERNAL_ERR;
				return NULL;
			}
		}
	}
	return NULL;
}

semanticky_typ check_identifier_type(SA_tree_node* node, psTree localStack) {
	if (exit_code == SUCCESS) {
		LA_token* identifier_token = node->token;
		psData identifierType = symTabSearch(&localStack, identifier_token->data.str);
		if (!identifierType) {
			identifierType = symTabSearch(&programTable, identifier_token->data.str);
			if (!identifierType) {
				exit_code = SEM_UNDEF_ERR;
			} else {
				identifierType->defined = true;

				//integer_var = integer_val, number_var = number_val, string_var = string_val
				if (identifierType->type != func) {
					return get_sem_type_from_s_type(identifierType->type);
				} else {
					SA_tree_node* expression_list = n_vec_get(node->nodes, 1);
					check_function_call(identifierType, expression_list, localStack);
					if (identifierType->returns) {
						if (identifierType->returns[0] == string_val) {
							return string_val_one_of;
						} else if (identifierType->returns[0] == number_val) {
							return number_val_one_of;
						} else if (identifierType->returns[0] == integer_val) {
							return integer_val_one_of;
						}
					} else {
						exit_code = SEM_EXPR_TYPE_ERR;
					}
				}
			}
		} else {
			identifierType->defined = true;
			//integer_var = integer_val, number_var = number_val, string_var = string_val
			if (identifierType->type != func) {
				return get_sem_type_from_s_type(identifierType->type);
			} else {
				exit_code = SEM_OTHER_ERR;
			}
		}
	}
	return -1;
}

bool is_defined_identifier(SA_tree_node* node, psTree localStack) {
	if (exit_code == SUCCESS) {
		LA_token* identifier_token = node->token;
		psData identifierType = symTabSearch(&localStack, identifier_token->data.str);
		if (!identifierType) {
			identifierType = symTabSearch(&programTable, identifier_token->data.str);
			if (!identifierType) {
				exit_code = SEM_UNDEF_ERR;
			} else {
				if (identifierType->type == func) {
					return true;
				}
				return identifierType->defined;
			}
		} else {
			if (identifierType->type == func) {
				return true;
			}
			return identifierType->defined;
		}
	}
	return false;
}

semanticky_typ* identifier_list_types(SA_tree_node* node, psTree localStack) {
	// line_of_code->identifier + identifier_contin
	// identifier_contin->identifier_list_inner + assignment + expression_list
	// identifier_list_inner ->
	// identifier_list_inner->comma + identifier + identifier_list_inner
	if (exit_code == SUCCESS) {

		// line_of_code->identifier + identifier_contin
		// identifier_contin->identifier_list_inner + assignment + expression_list
		SA_tree_node* identifier_contin = n_vec_get(node->nodes, 1);
		SA_tree_node* identifier_list_inner = n_vec_get_first(identifier_contin->nodes);

		unsigned size = 1 + count_inner_nodes(identifier_list_inner);

		// Memmory alloc for expression types list of function node
		semanticky_typ* identifier_types = (semanticky_typ*)malloc(sizeof(semanticky_typ) * size);
		if (identifier_types) {

			identifier_types[0] = check_identifier_type(n_vec_get_first(node->nodes), localStack);

			unsigned i = 1;
			identifier_list_inner = n_vec_get_first(identifier_contin->nodes);

			// While there are expressions
			while (identifier_list_inner->nodes->size) {

				// identifier_list_inner->comma + identifier + identifier_list_inner
				identifier_types[i] = check_identifier_type(n_vec_get(identifier_list_inner->nodes, 1), localStack);
				identifier_list_inner = n_vec_get(identifier_list_inner->nodes, 2);

				i++;
			}
			return identifier_types;
		} else {
			// Memmory allocate error
			exit_code = INTERNAL_ERR;
			return NULL;
		}
	}
	return NULL;
}

semanticky_typ check_expression(SA_tree_node* node, psTree localStack) {
	if (exit_code == SUCCESS) {
		// If nodes->size > 2 it means that it is one of next rules:
		// Expression -> expression + concat + expression
		// Expression -> expression + plus + expression
		// Expression -> expression + minus + expression
		// Expression -> expression + division + expression
		// Expression -> expression + whole_division + expression
		// Expression -> expression + times + expression
		// Expression -> l_bracket + expression + r_bracket
		if (node->nodes->size > 2) {
			SA_tree_node* middleNode = n_vec_get(node->nodes, 1);
			// Expression->l_bracket + expression + r_bracket
			if (middleNode->type == N_expression) {
				return check_expression(middleNode, localStack);
			} else {
				LA_token* op = middleNode->token;
				SA_tree_node* expression1 = n_vec_get(node->nodes, 0);
				SA_tree_node* expression2 = n_vec_get(node->nodes, 2);
				semanticky_typ exp1_type = check_expression(expression1, localStack);
				semanticky_typ exp2_type = check_expression(expression2, localStack);
				if (exp1_type == nil_val || exp2_type == nil_val) {
					exit_code = SEM_NIL_ERR;
				}
				if (exit_code == SUCCESS) {
					if (op->type == concat) { // Expression->expression + concat + expression
						if ((exp1_type != string_val && exp1_type != string_val_one_of) || (exp2_type != string_val && exp2_type != string_val_one_of)) {
							exit_code = SEM_EXPR_TYPE_ERR; //sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
						} else {
							return string_val;
						}
					} else if (op->type == whole_div) { // Expression->expression + whole_division + expression
						if ((exp1_type != string_val && exp1_type != string_val_one_of) && (exp2_type != string_val && exp2_type != string_val_one_of)) {
							if (exp1_type == number_val || exp1_type == number_val_one_of) {
								typecast_to_type(integer_val, expression1, localStack);
							}
							if (exp2_type == number_val || exp2_type == number_val_one_of) {
								typecast_to_type(integer_val, expression2, localStack);
							}
							return integer_val;
						} else {
							//sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
							exit_code = SEM_EXPR_TYPE_ERR;
						}
					} else { // +, -, /, *
						if (exp1_type != string_val && exp2_type != string_val) {
							if ((exp1_type == number_val || exp1_type == number_val_one_of) || (exp2_type == number_val || exp2_type == number_val_one_of)) {
								if (exp1_type == integer_val || exp1_type == integer_val_one_of) {
									typecast_to_type(number_val, expression1, localStack);
								}
								if (exp2_type == integer_val || exp2_type == integer_val_one_of) {
									typecast_to_type(number_val, expression2, localStack);
								}
								return number_val;
							} else {
								return integer_val;
							}
						} else {
							exit_code = SEM_EXPR_TYPE_ERR; //sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
						}
					}
				}
			}
		}
		// if nodes->size <= 2 if means that it is one of next rules:
		// Expression -> identifier
		// Expression -> string
		// Expression -> integer
		// Expression -> double
		// Expression -> key_nil
		// Expression -> str_len + str_len_arg
		// Str_len_arg-> string
		// Str_len_arg-> identifier
		else {
			LA_token* first = n_vec_get_first(node->nodes)->token;
			if (first->type == identif) {
				if (!is_defined_identifier(n_vec_get_first(node->nodes), localStack)) {
					exit_code = SEM_UNDEF_ERR;
				}
				return check_identifier_type(n_vec_get_first(node->nodes), localStack);
			} else if (first->type == key_word) {
				if (first->data.key_word == kw_nil) {
					return nil_val;
				} else {
					return get_sem_type_from_token(first);
				}
			} else if (first->type == str_len) {
				SA_tree_node* str_len_arg_node = n_vec_get(node->nodes, 1);
				if (str_len_arg_node->type != N_expression) {
					LA_token* str_len_agr_token = n_vec_get_first(str_len_arg_node->nodes)->token;
					if (str_len_agr_token->type == string) {
						return integer_val;
					} else if (str_len_agr_token->type == identif) {
						semanticky_typ typ = check_identifier_type(n_vec_get_first(str_len_arg_node->nodes), localStack);
						if (typ == string_val || typ == string_val_one_of) {
							return integer_val;
						}
						exit_code = SEM_EXPR_TYPE_ERR;
						return -1;
					}
					exit_code = SEM_OTHER_ERR;
				} else {
					semanticky_typ str_len_arg_type = check_expression(str_len_arg_node, localStack);
					if (str_len_arg_type == string_val || str_len_arg_type == string_val_one_of) {
						return integer_val;
					} else {
						exit_code = SEM_EXPR_TYPE_ERR;
						return -1;
					}
				}
			} else {
				if (first->type == string) {
					return string_val;
				} else if (first->type == integer_value) {
					return integer_val;
				} else if (first->type == double_value) {
					return number_val;
				}
				exit_code = SEM_OTHER_ERR;
			}
		}
	}
	return -1;
}