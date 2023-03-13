/** 
 * @file lexikalni_analyza.c
 * @brief implementace scanneru. 
 * Funkce potřebné pro lexikální analýzu
 * @author Samuel Kuchta <xkucht11@stud.fit.vutbr.cz>
 */

#include "lexikalni_analyza.h"

 // spracovává jednotlivé znaky, a vrací tokeny. vraci true kdyz validní, a false kdyz chyba.
bool get_token(LA_token* token) {

  FSM_state state = start;
  inflatable_text z_b;
  inflatable_text* zdroj_buffer = &z_b;
  char znak;
  char *endptr;

  while(1) {
    switch (state) {
      case start:
        znak = getc(stdin);
        if (((znak >= '0') && (znak <='9')) || (((znak >= 'a') && (znak <= 'z')) || ((znak >= 'A') && (znak <= 'Z')) || (znak == '_'))) {
          if (!inflatable_text_init(zdroj_buffer)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          if ((znak >= '1') && (znak <='9')) {  // int
            state = integer_st;
          } else if (znak == '0') {  // 0
            state = zero;
          } else {  // identifikator nebo keyword
            state = identif_and_kw;
          }
        } else if (isspace(znak) || (znak == '\t') || (znak == '\n')) {  // bile znaky
          state = start;
        } else if (znak == '(') { state = bracket_open_st;
        } else if (znak == ')') { state = bracket_close_st;
        } else if (znak == ',') { state = comma_st;
        } else if (znak == '/') { state = division_st;
        } else if (znak == '*') { state = times_st;
        } else if (znak == '+') { state = plus_st;
        } else if (znak == '-') { state = minus_st;
        } else if (znak == EOF) { state = end_of_file;
        } else if (znak == ':') { state = definition_st;
        } else if (znak == '<') { state = less_st;
        } else if (znak == '>') { state = greater_st;
        } else if (znak == '~') { state = not_eq_begin;
        } else if (znak == '=') { state = assignment_st;
        } else if (znak == '.') { state = tecka_st;
        } else if (znak == '#') { state = str_len_st;
        } else if (znak == '"') { state = string_loop;
          if (!inflatable_text_init(zdroj_buffer)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
        } else {  // Error
          DELET(token, NULL, LEX_ERR);
          fprintf(stderr,"Wrong input: %c \n", znak);
          return false;
        }
      break;

      case integer_st:  // 12345
        znak = fgetc(stdin);
        if ((znak >= '0') && (znak <='9')) { 
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = integer_st;
        } else if (znak == '.') {                    
           if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = decimal_point;
        } else if ((znak == 'e') || (znak == 'E')) {
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }                  
          state = double_exponent_begin;
        } else {
          ungetc(znak,stdin);                    
          TT = integer_value;
          token->data.int_value = atoi(ZBS);
          inflatable_text_free(zdroj_buffer);
          return true;
        } 
      break;

      case zero: // 0000
        znak = fgetc(stdin);
        if (znak == '0') { 
          state = zero;
        } else if (znak == '.') {
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }              
          state = decimal_point;
        } else if ((znak >= '0') && (znak <='9')) { 
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = integer_st;
        } else {
          ungetc(znak,stdin);
          TT = integer_value;
          token->data.int_value = 0;
          inflatable_text_free(zdroj_buffer);                 
          return true;                   
        }         
      break;

      case decimal_point: // 12345.
        znak = fgetc(stdin);
        if ((znak >= '0') && (znak <= '9')) {
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = double_point_value;
        } else {
          DELET(token, zdroj_buffer, LEX_ERR);
          fprintf(stderr,"Wrong input: %c \n", znak);
          return false;
        }
      break;
      
      case double_point_value: // 12345.12345
        znak = fgetc(stdin);
        if ((znak >= '0') && (znak <= '9')) {
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = double_point_value;
        } else if ((znak == 'e') || (znak == 'E')) {
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = double_exponent_begin;
        } else {
          ungetc(znak,stdin);           
          TT = double_value;
          token->data.double_value = strtod(ZBS,&endptr);
          inflatable_text_free(zdroj_buffer);
          return true;
        }
      break;

      case double_exponent_begin: // 12345.12345E
        znak = fgetc(stdin); 
        if ((znak == '+') || (znak == '-')) {
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }                   
          state = double_exponent_sign;
        } else if ((znak >= '0') && (znak <= '9')) {
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = double_exponent_value;  
        } else {
          DELET(token, zdroj_buffer, LEX_ERR);
          fprintf(stderr,"Wrong input: %c \n", znak);
          return false;
        }
      break;

      case double_exponent_sign: // 12345.12345E-
        znak = fgetc(stdin);
        if ((znak >= '0') && (znak <= '9')) {
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = double_exponent_value;  
        } else {
          DELET(token, zdroj_buffer, LEX_ERR);
          fprintf(stderr,"Wrong input: %c \n", znak);
          return false;
        }
      break;

      case double_exponent_value: // 12345.12345E-23
        znak = fgetc(stdin);
        if ((znak >= '0') && (znak <= '9')) {
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }                  
          state = double_exponent_value;  
        } else {
          ungetc(znak,stdin);                                    
          TT = double_value;
          token->data.double_value = strtod(ZBS,&endptr);
          inflatable_text_free(zdroj_buffer);
          return true;
        }
      break;

      case identif_and_kw: // "abcdef"
        znak = getc(stdin);
        if (isdigit(znak) || ((znak >= 'a') && (znak <= 'z')) || ((znak >= 'A') && (znak <= 'Z')) || (znak == '_')) {
           if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = identif_and_kw;
        } else {
          ungetc(znak,stdin);
          TT = key_word;                 
          if (strcmp(ZBS,"if") == 0) {
            KWRD = kw_if;
          } else if (strcmp(ZBS,"then") == 0) {       KWRD = kw_then;
          } else if (strcmp(ZBS,"else") == 0) {       KWRD = kw_else;
          } else if (strcmp(ZBS,"end") == 0) {        KWRD = kw_end;
          } else if (strcmp(ZBS,"do") == 0) {         KWRD = kw_do;          
          } else if (strcmp(ZBS,"while") == 0) {      KWRD = kw_while;
          } else if (strcmp(ZBS,"function") == 0) {   KWRD = kw_function;
          } else if (strcmp(ZBS,"return") == 0) {     KWRD = kw_return;
          } else if (strcmp(ZBS,"_") == 0) {          KWRD = kw_underscore;
          } else if (strcmp(ZBS,"local") == 0) {      KWRD = kw_local;
          } else if (strcmp(ZBS,"global") == 0) {     KWRD = kw_global;
          } else if (strcmp(ZBS,"nil") == 0) {        KWRD = kw_nil;
          } else if (strcmp(ZBS,"number") == 0) {     KWRD = kw_number;
          } else if (strcmp(ZBS,"integer") == 0) {    KWRD = kw_integer;
          } else if (strcmp(ZBS,"string") == 0) {     KWRD = kw_string;
          } else if (strcmp(ZBS,"require") == 0) {    KWRD = kw_require;
          } else {                        
            if (!store_identif_or_string_to_token(token, zdroj_buffer)) {
              DELET(token, zdroj_buffer, INTERNAL_ERR);
              return false;
            }
            TT = identif;
          }
          inflatable_text_free(zdroj_buffer);
          return true;
        }
      break;

      case bracket_open_st: // (
        TT = bracket_open;
        return true;
      break;

      case bracket_close_st: // )
        TT = bracket_close;
        return true;
      break;


      case comma_st: // ,
        TT = comma;
        return true;
      break;

      case division_st: // /
        znak = getc(stdin);
        if (znak == '/') {
          state = whole_div_st;
        } else {
          ungetc(znak,stdin);
          TT = division;
          return true;
        }        
      break;

      case whole_div_st: // //
        TT = whole_div;
        return true;
      break;

      case times_st: // *
        TT = times;
        return true;
      break;

      case plus_st: // +
        TT = plus;
        return true;
      break;

      case minus_st: // -
        znak = getc(stdin);
        if (znak == '-') {
          state = line_comment;
        } else {
        ungetc(znak,stdin);
        TT = minus;
        return true;
        }
      break;

      case line_comment: // --
        znak = getc(stdin);
        if (znak == '\n') {
          state = start;
        } else if (znak == EOF) {
          state = end_of_file;
        } else if (znak == '[') {
          state = line_to_block_comment;
        } else {
          state = line_comment;
        }
      break;

      case line_to_block_comment: // --[
        znak = getc(stdin);
        if (znak == '\n') {
          state = start;
        } else if (znak == EOF) {
          state = end_of_file;
        } else if (znak == '[') {
          state = block_comment_loop;
        } else {
          state = line_comment;
        }
      break;

      case block_comment_loop: // --[[
        znak = getc(stdin);
        if (znak == ']') {
          state = block_comment_end;
        } else if (znak == EOF) {
          state = end_of_file;
        } else {
          state = block_comment_loop;
        }
      break;

      case block_comment_end: // --[[]
        znak = getc(stdin);
        if (znak == ']') {
          state = start;
        } else if ( znak == EOF) {
          state = end_of_file;
        } else {
          state = block_comment_loop;
        }
      break;

      case end_of_file: // EOF
        TT = EoF;
        return true;                
      break;

      case definition_st: // :
        TT = definition;
        return true;
      break;

      case less_st: // <
        znak = getc(stdin);
        if (znak == '=') {
          state = less_eq_st;
        } else {
          ungetc(znak,stdin);
          TT = less;
          return true;
        }
      break;

      case less_eq_st: // <=
        TT = less_eq;
        return true;
      break;

      case greater_st: // >
        znak = getc(stdin);
        if (znak == '=') {
            state = greater_eq_st;
        } else {
          ungetc(znak,stdin);
          TT = greater;
          return true;
        }
      break;

      case greater_eq_st: // >=
        TT = greater_eq;
        return true;
      break;

      case not_eq_begin: // ~
        znak = getc(stdin);
        if (znak == '=') {
          state = not_eq_end;
        } else {
          DELET(token, NULL, LEX_ERR);
          fprintf(stderr,"Wrong input: %c \n", znak);
          return false;
        }
      break;

      case not_eq_end: // ~=
        TT = not_eq;
        return true;
      break;

      case assignment_st: // =
        znak = getc(stdin);
        if (znak == '=') {
          state = eq_st;
        } else {
          ungetc(znak,stdin);
          TT = assignment;
          return true;
        }
      break;

      case eq_st: // ==
        TT = eq;
        return true;
      break;

      case tecka_st: // .
        znak = getc(stdin);
        if (znak == '.') {
          state = concat_st;
        } else {
          DELET(token, NULL, LEX_ERR);
          fprintf(stderr,"Wrong input: %c \n", znak);
          return false;
        }
      break;

      case concat_st: // ..
        TT = concat;
        return true;
      break;

      case str_len_st: // #
        TT = str_len;
        return true;
      break;


      case string_loop:
        znak = getc(stdin);
        if (znak == '"') {
          state = string_end;
        } else if (znak == EOF) {
            state = string_wrong_input;
        } else if (znak == '\\') {  /*  92  = '\'  */
          state = escape_seq_begin;
        } else {
            if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }                
          state = string_loop;                    
        }
      break;

      case string_wrong_input:
        DELET(token, zdroj_buffer, LEX_ERR);
        fprintf(stderr,"Wrong input: %c \n", znak);
        return false;
      break;

      case escape_seq_begin:
        znak = getc(stdin);
        if (isdigit(znak)) {  // \ddd
           if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = escape_seq;
        } else if (znak > 31) {
          if (znak == 't' || znak == 'n' || znak == '"' || znak == '\\') {
            switch (znak) {
              case 't': znak = '\t'; break;
              case 'n': znak = '\n'; break;
              case '"': znak = '\"'; break;
              case '\\': znak = '\\'; break;
            }
          }
          if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
          state = string_loop;
        } else {
          DELET(token, zdroj_buffer, LEX_ERR);
          fprintf(stderr,"Wrong input: %c \n", znak);
          return false;
        }
      break;

      case escape_seq:
        znak = getc(stdin);
        if (isdigit(znak)) {
           if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }                  
          state = escape_seq_number;
        } else {
          DELET(token, zdroj_buffer, LEX_ERR);
          fprintf(stderr,"Wrong input: %c \n", znak);
          return false;
        }          
      break;     

      case escape_seq_number:
        znak = getc(stdin);
        if (isdigit(znak)) {
           if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
            // convert char to number
          int escape_seq_num = convert_esc_to_num(inflatable_text_read_char(zdroj_buffer, 2), inflatable_text_read_char(zdroj_buffer, 1),  inflatable_text_read_char(zdroj_buffer, 0));
          inflatable_text_sub_char(zdroj_buffer);
          inflatable_text_sub_char(zdroj_buffer);
          inflatable_text_sub_char(zdroj_buffer);
          if ((escape_seq_num > 31 || escape_seq_num == 9 || escape_seq_num == 10 || escape_seq_num == 34 || escape_seq_num == 92) && escape_seq_num < 256) {  /*  9 = '\t', 10 = '\n', 34 = '\"', 92 = '\\'  */
            znak = (char) escape_seq_num;
           if (!inflatable_text_add_char(zdroj_buffer,znak)) {
            DELET(token, zdroj_buffer, INTERNAL_ERR);
            return false;
          }
            state = string_loop;                      
          } else {
            DELET(token, zdroj_buffer, LEX_ERR);
            fprintf(stderr,"Incorrect escape sequence\n");
            return false;
          }
        } else {
          DELET(token, zdroj_buffer, LEX_ERR);
          fprintf(stderr,"Wrong input: %c \n", znak);
          return false;
        }     
      break;     

      case string_end:
        if (!store_identif_or_string_to_token(token, zdroj_buffer)) {
          DELET(token, zdroj_buffer, INTERNAL_ERR);
          return false;
        }
        TT = string;
        inflatable_text_free(zdroj_buffer);
        return true;
      break;
    }  // end of SWITCH
  }  // end of WHILE      
}  // end of get_token 


LA_token* create_token() {
  LA_token* token = (LA_token*) malloc(sizeof(LA_token));
  TT = -1;
  return token;
}

void delete_token(LA_token* token) {
  if ((TT == identif) || (TT == string)) {
    free(token->data.str);
  }
  free(token);
}

bool store_identif_or_string_to_token(LA_token* token, inflatable_text* zdroj_buffer) {
  if ((token->data.str = (char*) malloc(zdroj_buffer->length * sizeof(char) + 1)) == NULL) {;  // +1 pro '\0'
    return false;        
  } else {
    strcpy(token->data.str,ZBS);
    return true;
  }
}

int convert_esc_to_num(char hundred, char ten, char one) {
  int decimal_hundred = 100 * (int)(hundred - 48);
  int decimal_ten = 10 * (int)(ten - 48);
  int decimal_one = (int)(one - 48);
  return (decimal_hundred + decimal_ten + decimal_one);
}

void DELET(LA_token* token, inflatable_text* zdroj_buffer, int Error) {
    if (token != NULL) {
      delete_token(token);
    }
    if (zdroj_buffer != NULL) {
      inflatable_text_free(zdroj_buffer);
    }
    exit_code = Error;
}