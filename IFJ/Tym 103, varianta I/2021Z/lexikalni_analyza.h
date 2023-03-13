/** 
 * @file lexikalni_analyza.h
 * @brief implementace scanneru. 
 * Funkce potřebné pro lexikální analýzu
 * @author Samuel Kuchta <xkucht11@stud.fit.vutbr.cz>
 */

#ifndef _LEXIKALNI_ANALYZA_H
  #define _LEXIKALNI_ANALYZA_H
  #include "knihovny.h"

  #define KWRD (token->data.key_word)
  #define TT (token->type)
  #define ZBS (zdroj_buffer->str)

  typedef enum { // Typy tokenů. LA_token->token_type       
    integer_value,    // 0| 5
    double_value,     // 1| 5.5

    identif,          // 2| promenne/funkce
    key_word,         // 3| if,while...

    bracket_open,     // 4| (
    bracket_close,    // 5| )

    comma,            // 6| ,

    division,         // 7| /
    whole_div,        // 8| //
    times,            // 9| *
    plus,             // 10| +
    minus,            // 11| -

    EoF,              // 12| ctrl+D (linux)
    definition,       // 13| :

    less,             // 14| <
    less_eq,          // 15| <=

    greater,          // 16| >
    greater_eq,       // 17| >=

    not_eq,           // 18| ~=

    assignment,       // 19| =
    eq,               // 20| ==

    concat,           // 21| ..
    str_len,          // 22| #

    string            // 23| "text"
  } token_type;

  typedef enum{ // Keywordy
    kw_if,      
    kw_else,
    kw_then,
    kw_end,

    kw_do,
    kw_while,

    kw_function,
    kw_return,

    kw_underscore,  // '_' ked zahadzujeme vysledok funkcie

    kw_local,
    kw_global,

    kw_nil,
    kw_number,
    kw_integer,
    kw_string,

    kw_require
  } key_word_value;

  typedef union { // data nekterych tokenu.
    int64_t int_value; // cele cislo.
    double double_value; // desetinne cislo.
    char *str;  // identifikator nebo string.
    key_word_value key_word; // keyword.
  } token_data;

  typedef struct { // Struktura Tokenu
    token_type type;
    token_data data;
  } LA_token;

  typedef enum {
    start,

    integer_st,
    zero,
    decimal_point,
    double_point_value,
    double_exponent_begin,
    double_exponent_sign,
    double_exponent_value,

    identif_and_kw,

    bracket_open_st,
    bracket_close_st,

    comma_st,

    division_st,
    whole_div_st,

    times_st,
    plus_st,

    minus_st,
    line_comment,
    line_to_block_comment,
    block_comment_loop,
    block_comment_end,

    end_of_file,
    definition_st,

    less_st,
    less_eq_st,

    greater_st,
    greater_eq_st,

    not_eq_begin,
    not_eq_end,

    assignment_st,
    eq_st,

    tecka_st,
    concat_st,
    str_len_st,

    string_loop,
    string_wrong_input,
    escape_seq_begin,
    escape_seq,
    escape_seq_number,  
    string_end
  } FSM_state;

  bool get_token(LA_token* token_ptr); // precte data ze stdin, a vrati token.
  LA_token* create_token(); // alokuje pamet pro strukturu Token.
  void delete_token(LA_token* token_ptr); // uvolni pamet struktury, popripade i stringu nebo identifikatoru v ni.

  bool store_identif_or_string_to_token(LA_token* token_ptr, inflatable_text* zdroj_buffer); // ulozi string nebo identifikator do tokenu.
  int convert_esc_to_num(char hundred, char ten, char one); // konvertuje escape sekvenci na cislo

  void DELET(LA_token* token_ptr, inflatable_text* zdroj_buffer, int Error);

#endif  // LEXIKALNI_ANALYZA_H
