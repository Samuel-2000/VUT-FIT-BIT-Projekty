/** 
 * @file knihovny.h
 * @brief implementace scanneru. 
 * Funkce potřebné pro lexikální analýzu, generátor kódu, a exit_code
 * @author Samuel Kuchta <xkucht11@stud.fit.vutbr.cz>
 */

#ifndef _KNIHOVNY_H
  #define _KNIHOVNY_H

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h>
  #include <stdint.h>
  #include <ctype.h>

  extern int exit_code;
  
    // Definice konstant.
  #define SUCCESS 0  // Bezchybný překlad zdrojového kódu.

  #define LEX_ERR 1  // Chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).

  #define SYNTAX_ERR 2  // Chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu).

  #define SEM_UNDEF_ERR 3  // Sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici proměnné, atp.
  #define SEM_ASS_TYPE_ERR 4  // Sémantická chyba v příkazu přiřazení (typová nekompatibilita).
  #define SEM_FUN_PARAM_ERR 5  // Sémantická chyba v programu – špatný počet/typ parametrů či návratových hodnotu volání funkce či návratu z funkce.
  #define SEM_EXPR_TYPE_ERR 6  // Sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
  #define SEM_OTHER_ERR 7  // Ostatní sémantické chyby.
  #define SEM_NIL_ERR 8  // Běhová chyba při práci s neočekávanou hodnotou nil.
  #define SEM_DIV_BY_ZERO_ERR 9  // Běhová chyba celočíselného dělení nulovou konstantou.

  #define INTERNAL_ERR 99  // Interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.)



  #define INC_INFLATABLE_TEXT_LENGTH 16

  typedef struct {
    char* str;  // retezec ukonceny '\0'.
    unsigned int length;
    unsigned int alloc_size;
  } inflatable_text;

  bool inflatable_text_init(inflatable_text* s);

  bool inflatable_text_add_char(inflatable_text* s, char c);
  bool inflatable_text_sub_char(inflatable_text* s);

  char inflatable_text_read_char(inflatable_text* s, int offset);

  bool inflatable_text_add_const_str(inflatable_text* s, const char* const_string);

  void inflatable_text_clear(inflatable_text* s);
  void inflatable_text_free(inflatable_text* s);

#endif  // _KNIHOVNY_H