/**
 * @file generator_mezikodu.c
 * @brief implementace generátoru mezikódu.
 * Funkce potřebné pro generátoru mezikódu.
 * @author Samuel Kuchta <xkucht11@stud.fit.vutbr.cz>
 * @author Daria Dmitrievich <xdmitr00@stud.fit.vutbr.cz>
 */

#include "generator_mezikodu.h"

inflatable_text c_b; // retezec pro ukladani generovaneho mezikodu.
inflatable_text* code_buffer = &c_b;
char str[DIGIT_LIMIT];


 // //  //  //               Inicializace / Ukonceni
bool code_gen_begin() {
  if ((inflatable_text_init(code_buffer)) && (gen_file_header())) {
    return true;
  }    
  return false;
}

void code_gen_flush(FILE* destination_file) {
  fputs(code_buffer->str, destination_file);
  inflatable_text_free(code_buffer);
}

 // //  //  //               Promenne
bool gen_var_declare(char* var_id) {
  APPEND("DEFVAR LF@");
  APPEND(var_id);
  APPEND("\n");
  return true;
}

bool gen_var_default_value(char* var_id, token_type type) {
  APPEND("MOVE LF@");
  APPEND(var_id);
  APPEND(" ");
  if (!gen_default_var_value(type)) return false;
  return true;
}

 // //  //  //               Vetveni
bool gen_if_beginning(char* function_id, int label_depth, int label_index) {
  APPEND("\n#\tIf\n");
  APPEND("JUMPIFEQ ");
  APPEND(function_id);
  APPEND("%");
  APPEND_INT(label_depth);
  APPEND("%");
  APPEND_INT(label_index);
  APPEND(" GF@exp_result bool@false\n");
  return true;
}

bool gen_else(char* function_id, int label_depth, int label_index) {
  APPEND("JUMP ");
  APPEND(function_id);
  APPEND("%");
  APPEND_INT(label_depth);
  APPEND("%");
  APPEND_INT(label_index + 1);
  APPEND("\n#\tElse\n");
  if (!gen_label(function_id, label_index, label_depth)) return false;
  return true;
}

bool gen_if_end(char* function_id, int label_depth, int label_index) {
  APPEND("#\tEnd If\n");
  if (!gen_label(function_id, label_index, label_depth)) return false;
  return true;
}
 // //  //  //               Cykly
bool gen_while_header(char* function_id, int label_depth, int label_index) {
  APPEND("\n#\tDo While\n");
  if (!gen_label(function_id, label_index, label_depth)) return false;
  return true;
}

bool gen_while_beginning(char* function_id, int label_depth, int label_index) {
  APPEND("JUMPIFEQ ");
  APPEND(function_id);
  APPEND("%");
  APPEND_INT(label_depth);
  APPEND("%");
  APPEND_INT(label_index);
  APPEND(" GF@exp_result bool@false\n");
  return true;
}

bool gen_while_end(char* function_id, int label_depth, int label_index) {
  APPEND("JUMP ");
  APPEND(function_id);
  APPEND("%");
  APPEND_INT(label_depth);
  APPEND("%");
  APPEND_INT(label_index - 1);
  APPEND("\n#\tLoop\n");
  if (!gen_label(function_id, label_index, label_depth)) return false;
  return true;
}

 // //  //  //               Zasobnik
bool gen_push(LA_token token) {
  APPEND("PUSHS ");
  if (!gen_term_value(token)) {
    return false;
  }
  APPEND("\n");
  return true;
}

bool gen_stack_op(Rules rule) {
  switch (rule) {
  case ADD:   APPEND("ADDS\n");   break;
  case SUB:   APPEND("SUBS\n");   break;
  case MUL:   APPEND("MULS\n");   break;
  case DIV:   APPEND("DIVS\n");   break;
  case IDIV:  APPEND("IDIVS\n");  break;
  case EQ:    APPEND("EQS\n");    break;
  case LT:    APPEND("LTS\n");    break;
  case GT:    APPEND("GTS\n");    break;

  case LE:
    APPEND("GTS\n");
    APPEND("NOTS\n");
    break;

  case GE: 
    APPEND("LTS\n");
    APPEND("NOTS\n");
    break;

  case NEQ:
    APPEND("EQS\n");
    APPEND("NOTS\n");
    break;

  default:
    break;
  }
  return true;
}

bool gen_concat_stack_strings() {
  APPEND("POPS GF@var3\n");
  APPEND("POPS GF@var2\n");
  APPEND("CONCAT GF@var1 GF@var2 GF@var3\n");
  APPEND("PUSHS GF@var1\n");
  return true;
}

bool gen_save_expression_result(char* var_id, token_type ret_type, token_type l_type, char* frame) {
  if (l_type == integer_value && ret_type == double_value) {
    APPEND("FLOAT2INTS\n");
  } else if (l_type == double_value && ret_type == integer_value) {
    APPEND("INT2FLOATS\n");
  }
  APPEND("POPS ");
  APPEND(frame);
  APPEND("@");
  APPEND(var_id);
  APPEND("\n");
  return true;
}

 // //  //  //               Konverze na zasobniku
bool gen_stack_op1_to_double() {
  APPEND("INT2FLOATS\n");
  return true;
}

bool gen_stack_op1_to_integer() {
  APPEND("FLOAT2INTS\n");
  return true;
}

bool gen_stack_op2_to_double() {
  APPEND("POPS GF@var1\n");
  APPEND("INT2FLOATS\n");
  APPEND("PUSHS GF@var1\n");
  return true;
}

bool gen_stack_op2_to_integer() {
  APPEND("POPS GF@var1\n");
  APPEND("FLOAT2INTS\n");
  APPEND("PUSHS GF@var1\n");
  return true;
}

 // //  //  //               Vstup/Vystup
bool gen_input(char* var_id, token_type type) {
  APPEND("WRITE GF@input_prompt\n");
  APPEND("READ LF@");
  APPEND(var_id);
  switch (type) {
  case integer_value:
    APPEND(" int");
    break;
  case double_value:
    APPEND(" float");
    break;
  case string:
    APPEND(" string");
    break;
  default:
    return false;
  }
  APPEND("\n");
  return true;
}

bool gen_print() {
  APPEND("WRITE GF@exp_result\n");
  return true;
}
 // //  //  //               Funkce main
bool gen_main_beginning() {
  APPEND("\n#\tMain function.\n");
  APPEND("\tLABEL main\n");
  APPEND("CREATEFRAME\n");
  APPEND("PUSHFRAME\n");
  return true;
}

bool gen_main_scope_end() {
  APPEND("\n#\tEnd of main scope\n");
  APPEND("POPFRAME\n");
  APPEND("CLEARS\n");
  return true;
}

 // //  //  //               Uzivatelske funkce
bool gen_function_frame() {
  APPEND("CREATEFRAME\n");
  return true;
}

bool gen_function_pass_param(LA_token token, int index) {
  APPEND("DEFVAR TF@");
  APPEND_INT(index);
  APPEND("\nMOVE TF@");
  APPEND_INT(index);
  APPEND(" ");
  if (!gen_term_value(token)) return false;
  APPEND("\n");
  return true;
}

bool gen_function_convert_passed_param(token_type from, token_type to, int index) {
  if (from == double_value && to == integer_value) {
    APPEND("FLOAT2INT TF@");
    APPEND_INT(index);
    APPEND(" TF@");
    APPEND_INT(index);
    APPEND("\n");
  } else if (from == integer_value && to == double_value) {
    APPEND("INT2FLOAT TF@");
    APPEND_INT(index);
    APPEND(" TF@");
    APPEND_INT(index);
    APPEND("\n");
  }
  return true;
}

bool gen_fun_beginning(char* function_id) {
  APPEND("\n#\tbeginning of function ");
  APPEND(function_id);
  APPEND("\nLABEL ");
  APPEND(function_id);
  APPEND("\nPUSHFRAME\n");
  return true;
}
bool gen_function_call(char* function_id) {
  APPEND("CALL ");
  APPEND(function_id);
  APPEND("\n");
  return true;
}

bool gen_function_retval(token_type type) {
  APPEND("DEFVAR LF@retval\n");
  APPEND("MOVE LF@retval ");
  if (!gen_default_var_value(type)) return false;
  return true;
}

bool gen_function_variable(char* param_id, int index) {
  APPEND("DEFVAR LF@");
  APPEND(param_id);
  APPEND("\nMOVE LF@");
  APPEND(param_id);
  APPEND(" LF@");
  APPEND_INT(index);
  APPEND("\n");
  return true;
}

bool gen_function_retval_assign(char* l_val_id, token_type l_type, token_type ret_type) {
  if (l_type == integer_value && ret_type == double_value) {
    APPEND("FLOAT2INT TF@retval TF@retval\n");
  } else if (l_type == double_value && ret_type == integer_value) {
    APPEND("INT2FLOAT TF@retval TF@retval\n");
  }
  APPEND("MOVE LF@");
  APPEND(l_val_id);
  APPEND(" TF@retval\n");
  return true;
}

bool gen_fun_end(char* function_id) {
  APPEND("\n#\tEnd of function ");
  APPEND(function_id);
  APPEND("\nLABEL ");
  APPEND(function_id);
  APPEND("ret_label\n");
  APPEND("POPFRAME\n");
  APPEND("RETURN\n");
  return true;
}

bool gen_function_return(char* function_id) {
  APPEND("MOVE LF@retval GF@exp_result\n");
  APPEND("JUMP ");
  APPEND(function_id);
  APPEND("ret_label\n");
  return true;
}

 // //  //  //               Pomocne funkce
bool gen_file_header() {
  APPEND("\n#\tbeginning of program\n");
  APPEND(".IFJcode21\n");
  APPEND("DEFVAR GF@input_prompt\n");
  APPEND("MOVE GF@input_prompt string@?\\032\n");
  APPEND("DEFVAR GF@var1\n");
  APPEND("DEFVAR GF@var2\n");
  APPEND("DEFVAR GF@var3\n");
  APPEND("DEFVAR GF@exp_result\n");
  APPEND("JUMP main\n");
  if (define_built_in_functions() == false) {
    return false;
  }
  return true;
}

bool gen_default_var_value(token_type type) {
  switch (type) {
    case integer_value: APPEND("int@0\n");      break;
    case double_value:  APPEND("float@0.0\n");  break;
    case string:        APPEND("string@\n");    break;
    // nil?
    default: return false;
  }
  return true;
}

bool gen_term_value(LA_token token) {
  char term_str[DIGIT_LIMIT];
  unsigned char c;
  inflatable_text tmp_string;
  if (!inflatable_text_init(& tmp_string)) {
    return false;
  }
  switch (token.type) {
    case integer_value:
      sprintf(term_str, "%ld", token.data.int_value);
      APPEND("int@");
      APPEND(term_str);
      break;

    case double_value:
      sprintf(term_str, "%g", token.data.double_value);
      APPEND("float@");
      APPEND(term_str);
      break;

    case string:
      for (int i = 0; (c = (unsigned char)(token.data.str[i])) != '\0'; i++) {
        if (c == '#' || c == '\\' || c <= 32 || !isprint(c)) {
          inflatable_text_add_char(&tmp_string, '\\');
          sprintf(term_str, "%03d", c);
          inflatable_text_add_const_str(&tmp_string, term_str);
        } else {
          inflatable_text_add_char(&tmp_string, c);
        }
      }
      APPEND("string@");
      APPEND(tmp_string.str);
      break;

    case identif:
      APPEND("LF@");
      APPEND(token.data.str);
      break;
    default:
      inflatable_text_free(& tmp_string);
      return false;
  }
  inflatable_text_free(& tmp_string);
  return true;
}

bool gen_label(char* function_id, int label_depth, int label_index) {
  APPEND("\tLABEL ");
  APPEND(function_id);
  APPEND("%");
  APPEND_INT(label_depth);
  APPEND("%");
  APPEND_INT(label_index);
  APPEND("\n");
  return true;
}

bool define_built_in_functions() {
    // Příkazy pro načítání hodnot:
  APPEND(FUNCTION_READS);  // function reads() : string
	APPEND(FUNCTION_READI);  // function readi() : integer
	APPEND(FUNCTION_READN);  // function readn() : number

    // Příkaz pro výpis hodnot:
  APPEND(FUNCTION_WRITE);  // function write ( term1 , term2 , ..., term𝑛 )

    // Vestavěné funkce pro práci s řetězci:
	APPEND(FUNCTION_SUBSTR);  // function substr(s : string, i : number, j : number) : string
	APPEND(FUNCTION_ORD);  // function ord(s : string, i : integer) : integer
	APPEND(FUNCTION_CHR);  // function chr(i : integer) : string

  APPEND(FUNCTION_LENGTH);  // pomocna funkce pro substr

	return true;
}