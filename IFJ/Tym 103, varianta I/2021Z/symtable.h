/** 
 * @file symtable.h
 * @brief tabulka symbolů 
 * Funkce potřebné pro tabulku symbolů
 * @author Daria Dmitrievich <xdmitr00@stud.fit.vutbr.cz>
 */

#ifndef _TABULKA_SYMBOLU_H
  #define _TABULKA_SYMBOLU_H
  #include "knihovny.h"

  #define SYMTABLE_STACK_CHUNK 64  // Po kolika blocích se má alokovat zásobník

  typedef enum {  // Typ identifikátoru.
    string_var,   // Identifikátor je proměnná typu string.
    integer_var,  // Identifikátor je proměnná typu integer.
    number_var,   // Identifikátor je proměnná typu number.
    func	        // Identifikátor je funkce.
  } sType;

  typedef enum {  //  Typy hodnot.
    integer_val,
    number_val,
    string_val,
    nil_val,
    integer_val_one_of,
    number_val_one_of,
    string_val_one_of,
  } semanticky_typ;

  typedef struct sData {      // Data v konkretním uzlu.
    sType type;					      // Typ identifikátoru.
    bool defined;				      // jestli už byl identifikátor definovaný.
    struct sTree *localFrame;	// Ukazatel na lokální tabulku symbolů v případě funkce.
    int params;					      // Počet parametrů v případě funkce.
    semanticky_typ* returns;  // pole typů returnů v případě identifikátoru funkce
    semanticky_typ* args;     // pole typů argumentů v případě identifikátoru funkce
  } *psData;

  typedef struct sTree {  // Uzel binárního stromu (tabulka symbolů).
    char *key;			      // Identifikátor.
    struct sData *data;	  // Data uzlu.
    struct sTree *lptr;	  // Levá větev stromu (menší klíč).
    struct sTree *rptr;	  // Pravá větev stromu (větší klíč).
  } *psTree;

  typedef struct sStack { // Zásobník pro procházení stromu.
    int size;	            // Velikost zásobníku.
    int top;	            // Index nejvyššího prvku.
    psTree* S;	          // Zásobník ukazatelů na položky.
  } *psStack;             // Zásobník procházení stromu.

    // BVS tabulka symbolů.
  void symTabInit(psTree *tree);  // Inicializuje tabulku symbolů (binární vyhledávací strom) 
  psData symTabSearch(psTree *tree, char *key);  // Podle klíče vyhledá uzel ve stromě 
  void symTabInsert(psTree *tree, char *key, psData data);  // Vloží do stromu nový uzel s hodnotou data
  void symTabDefvarPre(psTree tree);  // Zadefinuje všechny proměnné ve stromu a inicializuje na nil
  void symTabLefmostPre(psTree tree, psStack stack);  // Projde levou stranu stromu a po cestě definuje proměnné
  void symTabDispose(psTree *tree);  // Zruší celý strom (tabulku symbolů)

    // Zasobnik pro uzly, pushnute z BVS pri jeho prohledavani.
  void symStackInit(psStack *stack);  // Inicializace zásobníku.
  void symStackPush(psStack stack, psTree tree);  // Vloží položku na vrchol zásobníku
  psTree symStackPop(psStack stack);  // Vyjme položku z vrcholu zásobníku (NULL, pokud je zásobník prázdný)
  void symStackDispose(psStack *stack);  // Uvolní zásobník z paměti

#endif	// _TABULKA_SYMBOLU_H