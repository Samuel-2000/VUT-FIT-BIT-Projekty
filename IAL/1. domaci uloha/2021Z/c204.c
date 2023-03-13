
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char topChar;
	while (!Stack_IsEmpty(stack)) { // cyklus pres neprazdny zasobnik
		Stack_Top(stack, &topChar); // topChar = znak z vrcholu zasobniku
		Stack_Pop(stack); // odstranění znaku z vrcholu zásobníku
		if (topChar == '(') {
			break;
		} 
		postfixExpression[(*postfixExpressionLength)++] = topChar; // umístění znaku z vrcholu zásobníku na konec výstupního řetězce a inkrementace jeho délky
	}
}

void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	if (Stack_IsEmpty(stack)) { // zásobník je prázdný
		Stack_Push(stack, c); // vložení operátoru na vrchol zásobníku
		return;
	}
	char topChar;
	Stack_Top(stack, &topChar); // uložení znaku z vrcholu zásobníku
	if ((topChar == '(') || ((topChar == '+' || topChar == '-') && (c == '*' || c == '/'))) { // na vrcholu zásobníku je operátor s nižší prioritou
		Stack_Push(stack, c); // vložení operátoru na vrchol zásobníku
		return;
	}
	postfixExpression[(*postfixExpressionLength)++] = topChar;	// umístění znaku z vrcholu zásobníku na konec výstupního řetězce a inkrementace jeho délky
	Stack_Pop(stack); // odstranění znaku z vrcholu zásobníku
	doOperation(stack, c, postfixExpression, postfixExpressionLength);	// tato funkce se bude rekuzivně volat, dokud se daný operátor nepodaří vložit na vrchol zásobníku
}

char *infix2postfix( const char *infixExpression ) {

	unsigned infIndex = 0, postIndex = 0;
	char *postExpr = malloc (sizeof (char) * MAX_LEN);
	Stack stack; 

	if (infixExpression == NULL || postExpr == NULL) {
    return NULL;
  }
	Stack_Init (&stack); // inicializace zasobniku
	while (TRUE) {
		if (infixExpression[infIndex] == '(') {
      Stack_Push (&stack, '(');
    } else if (infixExpression[infIndex] == ')') {
      untilLeftPar (&stack, postExpr, &postIndex);
    } else if (infixExpression[infIndex] == '+' || infixExpression[infIndex] == '-' || infixExpression[infIndex] == '*' || infixExpression[infIndex] == '/') {
      doOperation (&stack, infixExpression[infIndex], postExpr, &postIndex);
    } else if (infixExpression[infIndex] == '=') {
			untilLeftPar (&stack, postExpr, &postIndex);
			break;
		}	else {
			postExpr[postIndex] = infixExpression[infIndex];
			postIndex++;
		}
		infIndex++;
	}
	postExpr[postIndex] = '=';
	postExpr[postIndex + 1] = '\0';
	return postExpr;
}

/* Konec c204.c */
