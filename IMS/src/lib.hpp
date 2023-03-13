// Samuel Kuchta (xkucht11)

#pragma once

#include <unistd.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <stdbool.h>
#include <time.h>

using namespace std;

  // makrá
#define TSUNAMI_CHANCE 0.014
#define PIRATE_CHANCE 0.034
#define CRASH_CHANCE 0.07
#define TANKER_PRICE 220000000  // 100mil naklad, 120mil tanker

  // definuje hurikán na cestě.
#define HURRICANE(_wind_chance, _route_len)           \
  if (random_event(_wind_chance * args.wind_coef)) {  \
    if (random_event(175 * 100 / _route_len)) {       \
      if (random_event(0.2)) {                        \
        tanker_lost = true;                           \
        goto JMP_TANKER_LOST;                         \
      }                                               \
    } else {                                          \
      if (random_event(50)) {                         \
        km += 100;                                    \
        hours += 4;                                   \
      }                                               \
    }                                                 \
  }  // no hurricane

  // definuje hmlu na cestě.
#define HAZE(_haze_chance, _delay)                    \
  if (random_event(_haze_chance * args.haze_coef)) {  \
    hours += _delay;                                  \
    if (random_event(CRASH_CHANCE)) {                 \
      hours += 3;                                     \
    }                                                 \
  }  // no haze

  // definuje tsunami na cestě.
#define TSUNAMI                                           \
  if (random_event(TSUNAMI_CHANCE * args.tsunami_coef)) { \
    tanker_lost = true;                                   \
    goto JMP_TANKER_LOST;                                 \
  }  // no tsunami

  // definuje piráty na cestě.
#define PIRATES                                         \
  if (random_event(PIRATE_CHANCE * args.pirate_coef)) { \
    tanker_lost = true;                                 \
    goto JMP_TANKER_LOST;                               \
  }  // no pirates


#define A(_route_length)          \
  km += 4900;                     \
  hours += 189;                   \
  HURRICANE(1.92, _route_length)

#define B             \
  km += 850;          \
  hours += 33;

#define C       \
  km += 1010;   \
  hours += 39;  \
  HAZE(25, 1)

#define D       \
  km += 750;    \
  hours += 29;

#define E(_route_length)          \
  km += 850;                      \
  hours += 33;                    \
  HURRICANE(9.59, _route_length)

#define F       \
  km += 2210;   \
  hours += 85;  \
  PIRATES       \
  HAZE(60, 8)

#define G(_route_length)          \
  km += 1850;                     \
  hours += 71;                    \
  TSUNAMI                         \
  HURRICANE(0.082, _route_length)

#define H(_route_length)          \
  km += 2327;                     \
  hours += 90;                    \
  PIRATES                         \
  HURRICANE(0.15, _route_length)

#define I(_route_length)          \
  km += 4623;                     \
  hours += 179;                   \
  HURRICANE(0.45, _route_length)


#define USAGE(_prog_name) \
  cerr << "Použítí: " << _prog_name << " [-t <route>] [-c <count>] [-r <fuel_cost>] [-w <wind_coef>] " \
  << "[-t <tsunami_coef>] [-k <haze_coef>] [-p <pirate_coef>] [-v <verbose_output>] [-o <file_name>] " \
  << "\nSpusťí simulaci trasy se zadanými vstupními podmínkami.\n\n";  \
  return 1;

  // struktury
typedef struct opt_arg_values {
  uint8_t route = 2;
  uint8_t verbose_output = 0;
  uint32_t count = 10000000;  // 10mil
  double fuel_cost = 80.0;
  double wind_coef = 1.0;
  double haze_coef = 1.0;
  double tsunami_coef = 1.0;
  double pirate_coef = 1.0;
  char* filename_all;
  char* filename_avg;
} Arg;

  // funkce
void arg_parse(int argc, char** argv, Arg* args);
bool random_event(float chance);
