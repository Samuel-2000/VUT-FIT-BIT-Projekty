  /***************************************************************************************************************
 *|             Copyright (C) | Written by -> Samuel Kuchta <- | xkucht11@vutbr.cz | November 2019 |             |
  |                                                                                                              |
 *| v1.3 (29.11.2019) [11:46]                                                        This file is part of IZP.   |
  |                                                                                                              |
 *| Unauthorized copying of this file, without the express permission of Samuel Kuchta is strictly prohibited.   |
  |                                                                                                              |
 *| permission to copy this file is granted only to the VUT FIT lecturers.                                       |
  ***************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define I0 1e-12
#define UT 0.0258563

double calc_val(double Up, double U0, double R) {  // Spocita funkci Ip - Ir, ktere hodnota by se mela blizit k nule.
  return (I0 * ((exp(Up / UT)) - 1.0)) - ((U0 - Up) / R);
}

double  diode(double U0, double R, double Eps) {  // Pocita Up pomoci puleni intervalu.
  double Bot = 0.0; // Pocatek intervalu.
  double Top = U0;  // Konec intervalu.
  double Up = U0 / 2.0;  // Stred intervalu.
  double Val_up, Old_up;  // Funkcni hodnota ve stredu intervalu (Val_up).

  do {
    ((Val_up  = calc_val(Up, U0, R)) * calc_val(Bot, U0, R) < 0.0) ? (Top = Up) : (Bot = Up);  // Puleni intervalu na zaklade hodnot stredu, a dolni hranice intervalu.
      Old_up = Up;  // Ulozi stary stred intervalu pro pozdejsi porovnani.
      Up = (Bot + Top) / 2.0;  // Zmeni stred intervalu.
  } while (fabs(Val_up) > Eps && fabs(Old_up - Up) > Eps);  // Ukonci cyklus, jestli je splnena presnost podle hodnoty, nebo podle delky intervalu.
  return Up;  // Vrati vyhovujici stred intervalu.
}

int main(int argc, char* argv[]) {
  if (argc != 4) {  // V pripade nevyhovujiciho poctu argumentu ukonci program.
    fputs("\terror: invalid No. of arguments\n", stderr);
    return 1;
  }
  char* Ptr = NULL;  // Pro kontrolu, jestli na vstupu bylo zadano cislo a ne pismeno.
  double U0 = strtod(argv[1], &Ptr);  // Priradi ciselny argument do promenne.
  double R = strtod(argv[2], &Ptr);
  double Eps = strtod(argv[3], &Ptr);

  if (*Ptr != '\0' || U0 < 0.0 || R < 0.0 || Eps <= 0.0) {  // V pripade nevyhovujicich argumentu ukonci program.
    fputs("\nerror: invalid arguments\n", stderr);
    return 1;
  }
  double Up = diode(U0, R, Eps);  // Spocte hodnotu napeti pracovniho bodu diody.
  printf("\nUp = %g V\nIp = %g A\n\n", Up, (I0 * (exp(Up / UT) - 1.0)));  // Vytiskne Up a Ip.
  return 0;
}