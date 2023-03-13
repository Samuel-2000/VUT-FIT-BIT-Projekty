  /***************************************************************************************************************
 *|             Copyright (C) | Written by -> Samuel Kuchta <- | xkucht11@vutbr.cz | November 2019 |             |
  |                                                                                                              |
 *| v2.3 (12.11.2019) [22:46]                                                        This file is part of IZP.   |
  |                                                                                                              |
 *| Unauthorized copying of this file, without the express permission of Samuel Kuchta is strictly prohibited.   |
  |                                                                                                              |
 *| permission to copy this file is granted only to the VUT FIT lecturers.                                       |
  ***************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define SIZE (100) // Definice dlzky retezcu.

char* fun_find(char* Name, char* Temp_name) { // Funkce na filtrovani podle nazvu. Vrati cislo, ktere se pak bude porovnavat s argumentem.
  int Len = strlen(Name); // Deklarace promenne na nacteni dlzky retezce "Name", kvuli optimalizaci v nasledujicim For cyklu.
  memset(Temp_name, 0, strlen(Temp_name)); // Vynuluje retezec, aby tam nebyli hodnoty z predosleho kontaktu, protoze jinak by tisklo vsechny nasledujici kontakty.
  for (int i = 0; i < Len; i++) {
    Name[i] = tolower(Name[i]); // Nastavi znaky v retezci na male (Neni potreba mit ve switchi dvojnasobek poctu case).
    switch (Name[i]) { // Kazdemu znaku z "Name" priradi odpovidajici ciselnou hodnotu do "Temp_name".
      case 'a': case 'b': case 'c': strcat(Temp_name, "2"); break;
      case 'd': case 'e': case 'f': strcat(Temp_name, "3"); break;
      case 'g': case 'h': case 'i': strcat(Temp_name, "4"); break;
      case 'j': case 'k': case 'l': strcat(Temp_name, "5"); break;
      case 'm': case 'n': case 'o': strcat(Temp_name, "6"); break;
      case 'p': case 'q': case 'r': case 's': strcat(Temp_name, "7"); break;
      case 't': case 'u': case 'v': strcat(Temp_name, "8"); break;
      case 'w': case 'x': case 'y': case 'z': strcat(Temp_name, "9"); break;
      case '+': strcat(Temp_name, "0"); break;
      default: strcat(Temp_name, " "); break;
    }
  }
  return Temp_name;
}

void fun_show(char* Name, char* T_num, int* Print_cnt) { // Funkce na zobrazeni vystupu.
  Name[strlen(Name) - 1] = 0; // Zbavi sa znaku konce radku.
  printf("%s, %s", Name, T_num); // Vytiskne spojene retezce "Name" a "T_num".
  (*Print_cnt)++; // Znemozni vypsani "Not Found".
}

int main(int argc, char* argv[]) {
  printf("\n"); // Odsazeni kvuli prehlednosti pri behu programu.

  if (argc > 2) { // Chybovy vystup s privela argumentama. Ukonci program s chybovym hlasenim.
      fputs("\tPrivela argumentu\n", stderr);
      return 1;
  }
  
  char Name[SIZE], T_num[SIZE], Temp_name[SIZE]; // Inicializace promennych urcenych pro vstup ze souboru a filtrovani.
  int Print_cnt = 0; // Promenna pro urceni, jestli byl na vystupu zobrazen nejaky kontakt. V opacnem pripade se vypise "Not Found".

  for(;;) {
    if (fgets(Name, SIZE, stdin) && (fgets(T_num, SIZE, stdin) != NULL)) { // Nahrava kontakty ze vstupu, a zaroven kontroluje, jestli nenarazil na konec vstupu.
      if (argc == 1) { // Beh programu bez argumentu (nefiltruje).
        fun_show(Name, T_num, &Print_cnt); // Zobrazi vystup.
      } else { // Beh programu s argumentem (filtruje).
        if (strstr(T_num, argv[1]) || strstr(fun_find(Name, Temp_name), argv[1])) { // Vyfiltruje podle "T_num", a nebo vyfiltruje podle "Name".
          fun_show(Name, T_num, &Print_cnt); // Zobrazi vystup.
        }
      }
    } else { // Pri narazeni na konec vstupu ukonci program.
      if (Print_cnt == 0) {
        fputs("\tNot Found\n", stderr);
        return 1;
      }
      return 0;
    }
  }
}