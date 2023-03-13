/** 
 * @file knihovny.c
 * @brief implementace scanneru. 
 * Funkce potřebné pro lexikální analýzu a generátor kódu
 * @author Samuel Kuchta <xkucht11@stud.fit.vutbr.cz>
 */

#include "knihovny.h"

int exit_code = SUCCESS;

bool inflatable_text_init(inflatable_text* s) {
  if (!(s->str = (char*) malloc(INC_INFLATABLE_TEXT_LENGTH))) {
    return false;
  }
  inflatable_text_clear(s);
  s->alloc_size = INC_INFLATABLE_TEXT_LENGTH;
  return true;
}

void inflatable_text_clear(inflatable_text* s) {
  s->length = 0;
  s->str[s->length] = '\0';
}

void inflatable_text_free(inflatable_text* s) {
  free(s->str); // treba tento riadok?
}

bool inflatable_text_add_char(inflatable_text* s, char c) {
  if (s->length + 1 >= s->alloc_size) {
    unsigned int new_size = s->length + INC_INFLATABLE_TEXT_LENGTH + 1;
    if (!(s->str = (char*) realloc(s->str, new_size))) {
      return false;
    }
    s->alloc_size = new_size;
  }
  s->str[s->length++] = c;
  s->str[s->length] = '\0';
  return true;
}

bool inflatable_text_sub_char(inflatable_text* s) {
  s->length--;
  s->str[s->length] = '\0';
  return true;
}
char inflatable_text_read_char(inflatable_text* s, int offset) {
  return s->str[s->length - offset - 1];
}

bool inflatable_text_add_const_str(inflatable_text* s,  const char* const_string) {
  unsigned int const_str_length = (unsigned int) strlen(const_string);
  if (s->length + const_str_length + 1 >= s->alloc_size) {
    unsigned int new_size = s->length + const_str_length + 1;
    if (!(s->str = (char*) realloc(s->str, new_size))) {
      return false;
    }
    s->alloc_size = new_size;
  }
  s->length += const_str_length;
  strcat(s->str, const_string);
  s->str[s->length] = '\0';
  return true;
}