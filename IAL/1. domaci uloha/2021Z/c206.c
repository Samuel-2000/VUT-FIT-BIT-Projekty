
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

void DLL_Init( DLList *list ) { // Provede inicializaci seznamu list před jeho prvním použitím.
  list->activeElement = NULL;
  list->firstElement = NULL;
  list->lastElement = NULL;
}

void DLL_Dispose( DLList *list ) { // Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém
	DLLElementPtr HelpPtr = list->firstElement;
	while (list->firstElement != NULL) {
		list->firstElement = list->firstElement->nextElement;
		free(HelpPtr);
		HelpPtr = list->firstElement;
	}
	list->firstElement = NULL;
  list->lastElement = NULL;
  list->activeElement = NULL;
}

void DLL_InsertFirst( DLList *list, int data ) { // Vloží nový prvek na začátek seznamu list.
  DLLElementPtr NewPtr = (DLLElementPtr) malloc(sizeof(struct DLLElement));
  if (NewPtr == NULL) {
    DLL_Error();
    return;
  } else {
    NewPtr->data = data;
    NewPtr->nextElement = list->firstElement;
    NewPtr->previousElement = NULL;
    if (list->firstElement != NULL) {
      list->firstElement->previousElement = NewPtr;
    } else {
      list->lastElement = NewPtr;
    }
    list->firstElement = NewPtr;
  }
}

void DLL_InsertLast( DLList *list, int data ) { // Vloží nový prvek na konec seznamu list.	
  DLLElementPtr NewPtr = (DLLElementPtr) malloc(sizeof(struct DLLElement));
  if (NewPtr == NULL) {
    DLL_Error();
    return;
  }
  NewPtr->data = data;
  NewPtr->nextElement = NULL;
  NewPtr->previousElement = list->lastElement;
  if (list->lastElement != NULL) {
    list->lastElement->nextElement = NewPtr;
  } else {
    list->firstElement = NewPtr;
  }
  list->lastElement = NewPtr;
}

void DLL_First( DLList *list ) { // Nastaví aktivitu na první prvek seznamu list.
  list->activeElement = list->firstElement;
}


void DLL_Last( DLList *list ) { // Nastaví aktivitu na poslední prvek seznamu list. 
  list->activeElement = list->lastElement;
}

void DLL_GetFirst( DLList *list, int *dataPtr ) { // Prostřednictvím parametru data vrátí hodnotu prvního prvku seznamu list.
  if (list->firstElement == NULL) {
    DLL_Error();
    return;
  }
  *dataPtr = list->firstElement->data;
}

void DLL_GetLast( DLList *list, int *dataPtr ) { // Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
  if (list->lastElement == NULL) {
    DLL_Error();
    return;
  }
  *dataPtr = list->lastElement->data;
}

void DLL_DeleteFirst( DLList *list ) { // Zruší první prvek seznamu list. Pokud byl první prvek aktivní, aktivita 	
  DLLElementPtr HelpPtr;
  if (list->firstElement != NULL) {
    HelpPtr = list->firstElement;
    if (list->activeElement == list->firstElement) {
      list->activeElement = NULL;
    }
    if (list->firstElement == list->lastElement) {
      list->firstElement = NULL;
      list->lastElement = NULL;
    } else {
      list->firstElement = list->firstElement->nextElement;
      list->firstElement->previousElement = NULL;
    }
    free(HelpPtr);
  }
}

void DLL_DeleteLast( DLList *list ) { // Zruší poslední prvek seznamu list.
  DLLElementPtr helpptr;
  if (list->lastElement != NULL) {
    helpptr = list->lastElement;
    if (list->activeElement == list->lastElement) {
      list->activeElement = NULL;
    }
    if (list->lastElement == list->firstElement) {
      list->firstElement = NULL;
      list->lastElement = NULL;
    } else {
      list->lastElement = list->lastElement->previousElement;
      list->lastElement->nextElement = NULL;
    }
    free(helpptr);
  }
}

void DLL_DeleteAfter( DLList *list ) { // Zruší prvek seznamu list za aktivním prvkem.
  if (list->activeElement != NULL) {
    if (list->activeElement->nextElement != NULL) {
      DLLElementPtr HelpPtr = list->activeElement->nextElement;
      list->activeElement->nextElement = HelpPtr->nextElement;
      if (HelpPtr == list->lastElement) {
        list->lastElement = list->activeElement;
      } else {
        HelpPtr->nextElement->previousElement = list->activeElement;
      }
      free(HelpPtr);
    }
  }
}

void DLL_DeleteBefore( DLList *list ) { // Zruší prvek před aktivním prvkem seznamu list.
  if (list->activeElement != NULL) {
    if (list->activeElement->previousElement != NULL) {
      DLLElementPtr HelpPtr = list->activeElement->previousElement;
      list->activeElement->previousElement = HelpPtr->previousElement;
      if (HelpPtr == list->firstElement) {
        list->firstElement = list->activeElement;
      } else {
        HelpPtr->previousElement->nextElement = list->activeElement;
      }
      free(HelpPtr);
    }
  }
}

void DLL_InsertAfter( DLList *list, int data ) { // Vloží prvek za aktivní prvek seznamu list.
  if (list->activeElement != NULL) {
    DLLElementPtr NewPtr = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    if (!NewPtr) {
      DLL_Error();
      return;
    }
    NewPtr->data = data;
    NewPtr->nextElement = list->activeElement->nextElement;
    NewPtr->previousElement = list->activeElement;
    list->activeElement->nextElement = NewPtr;
    if (list->activeElement == list->lastElement) {
      list->lastElement = NewPtr;
    } else {
      list->activeElement->nextElement->previousElement = NewPtr;
    }
  }
}

void DLL_InsertBefore( DLList *list, int data ) {
  if (list->activeElement != NULL) {
    DLLElementPtr NewPtr = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    if (NewPtr == NULL) {
      DLL_Error();
      return;
    }
    NewPtr->data = data;
    NewPtr->nextElement = list->activeElement;
    NewPtr->previousElement = list->activeElement->previousElement;
    list->activeElement->previousElement = NewPtr;
    if (list->activeElement == list->firstElement) {
      list->firstElement = NewPtr;
    } else {
      list->activeElement->previousElement->nextElement = NewPtr;
    }
  }
}

void DLL_GetValue( DLList *list, int *dataPtr ) { // Prostřednictvím parametru data vrátí hodnotu aktivního prvku seznamu list.
  if (list->activeElement == NULL) {
    DLL_Error();
    return;
  }
  *dataPtr = list->activeElement->data;
}

void DLL_SetValue( DLList *list, int data ) { // Přepíše obsah aktivního prvku seznamu list.
  if (list->activeElement == NULL) {
    list->activeElement->data = data;
  }
}

void DLL_Next( DLList *list ) { // Posune aktivitu na následující prvek seznamu list.
  if (list->activeElement) {
    list->activeElement = list->activeElement->nextElement;
  }
}

void DLL_Previous( DLList *list ) { // Posune aktivitu na předchozí prvek seznamu list.
  list->activeElement = list->activeElement->previousElement;
}

int DLL_IsActive( DLList *list ) { // Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
  return list->activeElement != NULL;
}

/* Konec c206.c */
