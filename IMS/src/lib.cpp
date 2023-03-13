// Samuel Kuchta (xkucht11)
#include "lib.hpp"

void arg_parse(int argc, char** argv, Arg* args) {
  int opt;
  bool file_set = false;
  stringstream filename_temp;
  try {
    while ((opt = getopt(argc, argv, "t:c:r:w:k:s:p:v:o:")) != -1) {
      switch (opt) {
        case 't':
          args->route = atoi(optarg);
          filename_temp << (char) opt << optarg << "_";
          break;

        case 'c':
          args->count = atoi(optarg);
          filename_temp << (char) opt << optarg << "_";
          break;

        case 'r':
          args->fuel_cost = atoi(optarg);
          filename_temp << (char) opt << optarg << "_";
          break;

        case 'w':
          args->wind_coef = atoi(optarg);
          filename_temp << (char) opt << optarg << "_";
          break;

        case 'k':
          args->haze_coef = atoi(optarg);
          filename_temp << (char) opt << optarg << "_";
          break;

        case 's':
          args->tsunami_coef = atoi(optarg);
          filename_temp << (char) opt << optarg << "_";
          break;

        case 'p':
          args->pirate_coef = atoi(optarg);
          filename_temp << (char) opt << optarg << "_";
          break;

        case 'v':
          args->verbose_output = atoi(optarg);
          break;

        case 'o':
          file_set = true;
          args->filename_avg = (char*) malloc(sizeof(char) * (strlen(optarg) + 10));
          args->filename_all = (char*) malloc(sizeof(char) * (strlen(optarg) + 10));
          strcpy(args->filename_avg, optarg);
          strcpy(args->filename_all, optarg);
          strcat(args->filename_avg, "__AVG.txt");
          strcat(args->filename_all, "__ALL.txt");
          break;

        default:
          printf("\nUnsuported argument: %c\n", char(opt));
          break;
      }
    }
  } catch (runtime_error &error) {  // vykoná sa, ak nastala chyba/výnimka počas parsovania argumentov
    cerr << "[ERROR] " << error.what() << endl;  // vypíše, aká chyba nastala
  }
  if (
    args->route < 1         || args->route > 4            || 
    args->count < 1         || args->count > 1000000000   || 
    args->fuel_cost < 0     || args->fuel_cost > 1000     || 
    args->wind_coef < 0     || args->wind_coef > 1500     || 
    args->tsunami_coef < 0  || args->tsunami_coef > 8000  || 
    args->haze_coef < 0     || args->haze_coef > 5        || 
    args->pirate_coef < 0   || args->pirate_coef > 3000   || 
    args->verbose_output > 1) 
  {
    printf("\nWrong value of argument (Read Readme file)\n");
    exit(EXIT_FAILURE);
  } else {
    if (file_set == false) {
      args->filename_avg = (char*) malloc(sizeof(char) * (strlen(filename_temp.str().c_str()) + 10));
      args->filename_all = (char*) malloc(sizeof(char) * (strlen(filename_temp.str().c_str()) + 10));
      strcpy(args->filename_avg, filename_temp.str().c_str());
      strcpy(args->filename_all, filename_temp.str().c_str());
      strcat(args->filename_avg, "__AVG.txt");
      strcat(args->filename_all, "__ALL.txt");
    }
  }
}

bool random_event(float chance) {  // input chance between 0-100%. return true if random number is bigger than input.
  float percent = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));  // https://stackoverflow.com/questions/686353/random-float-number-generation
  if (chance > percent * 100) {
    return true;
  } else {
    return false;
  }
}
