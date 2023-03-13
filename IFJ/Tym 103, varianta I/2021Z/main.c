/** 
 * @file main.c
 * @brief implementace mainu. 
 * Funkce potřebné pro začátek a ukončení interpretu.
 * @author Samuel Kuchta <xkucht11@stud.fit.vutbr.cz>
 */


#include "syntakticka_analyza.h"

int main() {

   if ((code_gen_begin() && gen_main_beginning() && gen_main_scope_end()) == false) {  // vytvarim zaklad pro mezikod
    printf("code_gen error");
   }
  gen_function_frame();


  parser();  // volam syntakticku analyzu

  switch (exit_code) {
    case SUCCESS: break;
    case LEX_ERR:             printf("\nChyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).\n"); break;
    case SYNTAX_ERR:          printf("\nChyba v programu v rámci syntaktické analýzy (chybná syntaxe programu).\n"); break;
    case SEM_UNDEF_ERR:       printf("\nSémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici proměnné, atp.\n"); break;
    case SEM_ASS_TYPE_ERR:    printf("\nSémantická chyba v příkazu přiřazení (typová nekompatibilita).\n"); break;
    case SEM_FUN_PARAM_ERR:   printf("\nSémantická chyba v programu – špatný počet/typ parametrů či návratových hodnotu volání funkce či návratu z funkce.\n"); break;
    case SEM_EXPR_TYPE_ERR:   printf("\nSémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.\n"); break;
    case SEM_OTHER_ERR:       printf("\nOstatní sémantické chyby.\n"); break;
    case SEM_NIL_ERR:         printf("\nBěhová chyba při práci s neočekávanou hodnotou nil.\n"); break;
    case SEM_DIV_BY_ZERO_ERR: printf("\nBěhová chyba celočíselného dělení nulovou konstantou.\n"); break;
    case INTERNAL_ERR:        printf("\nInterní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd).\n"); break;
  }

  code_gen_flush(stdout);  // printuju vygenerovany mezikod

  return exit_code;
}