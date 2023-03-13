/**
 * @file generator_mezikodu.h
 * @brief implementace generátoru mezikódu.
 * Funkce potřebné pro generátoru mezikódu.
 * @author Samuel Kuchta <xkucht11@stud.fit.vutbr.cz>
 * @author Daria Dmitrievich <xdmitr00@stud.fit.vutbr.cz>
 */

#ifndef _GENERATOR_MEZIKODU_H
  #define _GENERATOR_MEZIKODU_H
  #include "lexikalni_analyza.h"


  #define DIGIT_LIMIT 48 // Nejvyssi pocet cisel pro konverzi cisla do retezce.
	

  	// pridani retezce do bufferu	
  #define APPEND(_code)	\
    if (!inflatable_text_add_const_str(code_buffer, (_code))) { \
      return false;						\
		}

  	// konverze cisla do retezce
  #define APPEND_INT(_code)   	\
    sprintf(str, "%d", _code);  \
    APPEND(str);

    // pravidla pro operace na zasobniku.
  typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    IDIV,
    EQ,
    NEQ,
    LT,
    GT,
    LE,
    GE
  } Rules;

    // Inicializace
  bool code_gen_begin();  // Inicializace.
  void code_gen_flush(FILE *destination_file);  // Ulozi vygenerovany mezikod ulozeny jako retezec v pameti do ciloveho souboru, a uvolni ho z pameti.
 
    // Promenne
  bool gen_var_declare(char *var_id);  // deklaruje promennou.
  bool gen_var_default_value(char *var_id, token_type type);  // priradi zakladni hodnotu do promenne (definice).
  
    // Vetveni
  bool gen_if_beginning(char *function_id, int label_depth, int label_index);  // zacatek if-then vyrazu.
  bool gen_else(char *function_id, int label_depth, int label_index);  // skok na else.
  bool gen_if_end(char *function_id, int label_depth, int label_index);  // end
  
    // Cykly
  bool gen_while_header(char *function_id, int label_depth, int label_index);  // hlavicka do-while.
  bool gen_while_beginning(char *function_id, int label_depth, int label_index);  // podmienka na preskoceni.
  bool gen_while_end(char *function_id, int label_depth, int label_index);  // telo cyklu.
  
    // Zasobnik
  bool gen_push(LA_token token);  // ulozeni na zasobnik.
  // bool gen_stack_op(Rules rule);  // operace na zasobniku.
  bool gen_concat_stack_strings();  // concatenace retezcovych hodnot na zasobniku.
  bool gen_save_expression_result(char *var_id, token_type ret_type, token_type l_type, char *frame);  // ulozi hodnotu na zasobniku do promenne v ramci.
  
    // Konverze na zasobniku
  bool gen_stack_op1_to_double();  // z int do float
  bool gen_stack_op1_to_integer();  // z float do int.
  bool gen_stack_op2_to_double();  // z int do float (2 operand v zasobniku).
  bool gen_stack_op2_to_integer();  // z float do int (2 operand v zasobniku).
  
    // vstup/vystup
  bool gen_input(char *var_id, token_type type);  // vyzve uzivatele k zadani vstupu.
  bool gen_print();  // vytiskne vysledek.
  
    // Funkce main
  bool gen_main_beginning();  // zacatek funkce main
  bool gen_main_scope_end();  // konec funkce main.
  
    // Uzivatelske funkce
  bool gen_function_frame();  // Vytvori TF.
  bool gen_function_pass_param(LA_token token, int index);  // definuje parametry pro funkci.
  bool gen_function_convert_passed_param(token_type from, token_type to, int index);  // konvertuje parametry.
  bool gen_fun_beginning(char *function_id);  // vytvori navesti funkce.
  bool gen_function_call(char *function_id);  // skok na navesti funkce.
  bool gen_function_retval(token_type type);  // definuje predvolenou navratovou hodnotu.
  bool gen_function_variable(char *param_id, int index);  // definuje lokalni promenne.
  bool gen_function_retval_assign(char *l_val_id, token_type l_type, token_type ret_type);  // definuje vysledek (navratovou hodnotu).
  bool gen_fun_end(char *function_id);  // konec funkce.
  bool gen_function_return(char *function_id); // navrat z funkce.
  
    // Pomocne funkce
  bool gen_file_header();  // hlavicka souboru, definice globalnich promennych, skok na funkci.
  bool gen_term_value(LA_token token);  // definuje hodnotu na zaklade datoveho typu.
  bool gen_default_var_value(token_type type);  // predvolena hodnota urcitych datovych typu
  bool gen_label(char* function_id, int label_depth, int label_index); // vytvori navesti
  bool define_built_in_functions();



      // Mezikodove definice vestavenych funkci, ktere budou v hlavicce programu, a mohou se volat instrukcii CALL
      // parametry si berou ze zasobniku, a navratovou hodnotu ukladaji do promenne retval.
    // Příkazy pro načítání hodnot:
  // function reads() : string
#define FUNCTION_READS              \
	"\n\t# Built-in function Reads\n" \
	"\tLABEL $Reads\n"			          \
	"PUSHFRAME\n"						          \
	"DEFVAR LF@retval\n"				      \
	"READ LF@retval string\n"         \
	"POPFRAME\n"						          \
	"RETURN\n"

 // function readi() : integer
#define FUNCTION_READI              \
	"\n\t# Built-in function Readi\n" \
	"\tLABEL $Readi\n"					      \
	"PUSHFRAME\n"						          \
	"DEFVAR LF@retval\n"				      \
	"READ LF@retval int\n"            \
	"POPFRAME\n"					          	\
	"RETURN\n"

  // function readn() : number
#define FUNCTION_READN              \
	"\n\t# Built-in function Readn\n" \
	"\tLABEL $Readn\n"					      \
	"PUSHFRAME\n"						          \
	"DEFVAR LF@retval\n"				      \
	"READ LF@retval float\n"   			  \
	"POPFRAME\n"						          \
	"RETURN\n"


  // Příkaz pro výpis hodnot:
  // function write ( term1 , term2 , ..., term𝑛 )
  // potrebuje mit na zasobniku ulozen pocet argumentu, a samotne argumenty
  // Vestavěný příkaz má libovolný počet parametrů tvořených termy oddělenými čárkou.
  // Sémantika příkazu je následující: Postupně zleva doprava prochází termy (podrobněji
  // popsány v sekci 3.1) a vypisuje jejich hodnoty na standardní výstup ihned za sebe bez
  // žádných oddělovačů dle typu v patřičném formátu. Za posledním termem se též nic
  // nevypisuje! Hodnota termu typu integer bude vytištěna pomocí '%d'16, hodnota
  // termu typu number pak pomocí '%a'17. Funkce write nemá návratovou hodnotu.
#define FUNCTION_WRITE                    \
	"\n\t# Built-in function Write\n"	      \
	"\tLABEL $Write\n"									    \
	"PUSHFRAME\n"												    \
	"DEFVAR LF@retval\n"								    \
	"MOVE LF@retval nil@nil\n"							\
	"DEFVAR LF@argc\n"										  \
	"POPS LF@argc\n"										    \
	"DEFVAR LF@lastArg\n"										\
	"DEFVAR LF@counter\n"										\
	"MOVE LF@counter int@0\n"								\
	"DEFVAR LF@string\n"										\
	"MOVE LF@string string@0\n"							\
	"DEFVAR LF@type\n"										  \
	"\tLABEL $while\n"										  \
	"JUMPIFEQ $end LF@counter LF@argc\n"		\
	"ADD LF@counter LF@counter int@1\n"			\
	"POPS LF@string\n"										  \
	"TYPE LF@type LF@string\n"							\
	"JUMPIFEQ $none LF@type string@nil\n"		\
	"WRITE LF@string\n"										  \
	"JUMP $endNone\n"										    \
	"\tLABEL $none\n"										    \
	"WRITE string@None\n"										\
	"\tLABEL $endNone\n"										\
	"LT LF@lastArg LF@counter LF@argc\n"		\
	"JUMPIFEQ $end LF@lastArg bool@false\n" \
	"JUMP $while\n"										      \
	"\tLABEL $end\n"										    \
	"POPFRAME\n"												    \
	"RETURN\n"

    // Vestavěné funkce pro práci s řetězci:
  // SubStr(s As String, i As Integer, n As Integer) As String
  // Vrátí
  // podřetězec zadaného řetězce 𝑠. Druhým parametrem 𝑖 je dán index začátku poža-
  // dovaného podřetězce (počítáno od jedničky) a třetím parametrem 𝑗 je index konce
  // požadovaného podřetězce (počítáno od jedničky). Je-li index 𝑖, nebo 𝑗 mimo meze
  // 1 až #s, nebo je-li 𝑗 < 𝑖, vrací funkce prázdný řetězec. Je-li některý parametr nil,
  // nastává chyba 8.
#define FUNCTION_SUBSTR														                  \
	"\n\t# Built-in function SubStr\n"											          \
	"\tLABEL $substr\n"														                    \
	"PUSHFRAME\n"															                        \
	"DEFVAR LF@retval\n"													                    \
	"MOVE LF@retval string@\n"											                  \
	"DEFVAR LF@length_str\n"												                  \
	"CREATEFRAME\n"														                        \
	"DEFVAR TF@var0\n"														                    \
	"MOVE TF@var0 LF@var0\n"													                \
	"CALL $length\n"														                      \
	"MOVE LF@length_str TF@retval\n"										              \
	"DEFVAR LF@ret_cond\n"													                  \
	"LT LF@ret_cond LF@length_str int@0\n"									          \
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"						      \
	"EQ LF@ret_cond LF@length_str int@0\n"								  	        \
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"						      \
	"LT LF@ret_cond LF@var1 int@0\n"										          	  \
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"						      \
	"EQ LF@ret_cond LF@var1 int@0\n"									            		\
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"						      \
	"GT LF@ret_cond LF@var1 LF@length_str\n"							        		\
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"						      \
	"EQ LF@ret_cond LF@var2 int@0\n"										              \
	"JUMPIFEQ $substr$return LF@ret_cond bool@true\n"						      \
	"DEFVAR LF@max_n\n"												                    	  \
	"MOVE LF@max_n LF@length_str\n"									              	  \
	"SUB LF@max_n LF@max_n LF@var1\n"									            	  \
	"ADD LF@max_n LF@max_n int@1\n"									            	    \
	"DEFVAR LF@edit_n_cond\n"											                	  \
	"LT LF@edit_n_cond LF@var2 int@0\n"									        	    \
	"JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true\n"					    \
	"GT LF@edit_n_cond LF@var2 LF@max_n\n"								          	\
	"JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true\n"				    	\
	"JUMP $substr$process\n"											              	    \
	"\tLABEL $substr$edit_n\n"											                  \
	"MOVE LF@var2 LF@max_n\n"											                    \
	"\tLABEL $substr$process\n"									                      \
	"DEFVAR LF@index\n"												                  	    \
	"MOVE LF@index LF@var1\n"										                    	\
	"SUB LF@index LF@index int@1\n"						                  			\
	"DEFVAR LF@char\n"											              			      \
	"DEFVAR LF@process_loop_cond\n"						                      	\
	"\tLABEL $substr$process_loop\n"											            \
	"GETCHAR LF@char LF@var0 LF@index\n"										          \
	"CONCAT LF@retval LF@retval LF@char\n"								            \
	"ADD LF@index LF@index int@1\n"										                \
	"SUB LF@var2 LF@var2 int@1\n"												              \
	"GT LF@process_loop_cond LF@var2 int@0\n"							            \
	"JUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true\n"	\
	"\tLABEL $substr$return\n"												                \
	"POPFRAME\n"															                        \
	"RETURN\n"

  // function ord(s : string, i : integer) : integer
  // vrátí ordinální hodnotu
  // (ASCII) znaku na pozici 𝑖 v řetězci 𝑠. Je-li jeden z parametrů nil, nastává chyba 8.
  // Je-li index 𝑖 mimo meze řetězce (1 až #s), vrací funkce nil.
#define FUNCTION_ORD                                \
	"\n\t# Built-in function Ord\n"	                  \
	"\tLABEL $Ord\n"									                \
	"PUSHFRAME\n"												              \
	"DEFVAR LF@retval\n"								              \
	"MOVE LF@retval nil@nil\n"					              \
	"DEFVAR LF@s\n"					                          \
	"POPS LF@s\n"					                            \
	"DEFVAR LF@cond\n"					                      \
	"TYPE LF@cond LF@s\n"					                    \
	"JUMPIFEQ $ord$isString LF@cond string@string\n"  \
	"EXIT int@4\n"					                          \
	"LABEL $ord$isString\n"					                  \
	"DEFVAR LF@i\n"					                          \
	"POPS LF@i\n"					                            \
	"DEFVAR LF@len\n"					                        \
	"STRLEN LF@len LF@s\n"					                  \
	"LT LF@cond LF@i int@0\n"					                \
	"JUMPIFEQ $ord$end LF@cond bool@true\n"					  \
	"LT LF@cond LF@i LF@len\n"					              \
	"JUMPIFEQ $end LF@cond bool@false\n"					    \
	"STRI2INT LF@retval LF@s LF@i\n"					        \
	"LABEL $ord$end\n"					                      \
	"POPFRAME\n"												              \
	"RETURN\n"

  // function chr(i : integer) : string
  // Vrátí jednoznakový řetězec se znakem,
  // jehož ASCII kód je zadán parametrem 𝑖. Případ, kdy je 𝑖 mimo interval [0; 255], vede
  // na hodnotu nil. Je-li 𝑖 nil, nastává chyba 8.
#define FUNCTION_CHR														    \
	"\n\t# Built-in function Chr\n"										\
	"\tLABEL $chr\n"															    \
	"PUSHFRAME\n"															        \
	"DEFVAR LF@retval\n"													    \
	"MOVE LF@retval string@\n"											  \
	"DEFVAR LF@cond_range\n"												  \
	"LT LF@cond_range LF@var0 int@0\n"								\
	"JUMPIFEQ $chr$return LF@cond_range bool@true\n"	\
	"GT LF@cond_range LF@var0 int@255\n"							\
	"JUMPIFEQ $chr$return LF@cond_range bool@true\n"  \
	"INT2CHAR LF@retval LF@var0\n"									  \
	"\tLABEL $chr$return\n"													  \
	"POPFRAME\n"															        \
	"RETURN\n"

    // function Length(s : String) : Integer (pomocna funkce pro substr)
  #define FUNCTION_LENGTH							\
	"\n\t# Built-in function Length\n"  \
	"\tLABEL $length\n"									\
	"PUSHFRAME\n"												\
	"DEFVAR LF@retval\n"								\
	"STRLEN LF@retval LF@var0\n"				\
	"POPFRAME\n"												\
	"RETURN\n"

#endif  // _GENERATOR_MEZIKODU_H