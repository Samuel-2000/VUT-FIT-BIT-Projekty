// Samuel Kuchta (xkucht11)
// Simulace Námořní dopravy IMS projekt 2022

#include "lib.hpp"

int main(int argc, char **argv) {
  Arg args;
  arg_parse(argc, argv, &args);

  srand (time(NULL));  // initialize random seed.

  uint16_t km;  // pocet ujetych kilometru
  uint16_t hours;  // pocet hodin plavby
  bool tanker_lost;
  double price = 0.0;

  uint64_t total_km = 0;
  uint64_t total_hours = 0;
  uint32_t tankers_lost = 0;
  uint64_t total_price = 0;

  double avg_km;  // pocet ujetych kilometru
  double avg_hours;  // pocet hodin plavby
  double avg_price;

  ofstream route_info_avg;
  ofstream route_info_all;

  if (args.verbose_output) {
    route_info_all.open(args.filename_all, ios::out);
    route_info_all.precision(9);
  }
  
  for (uint32_t i = 0; i < args.count; i++) {
    km = 0;
    hours = 0;
    tanker_lost = false;

    if (args.route == 1) {  // Thajský kanál
      A(8360)
      B
      C
      D
      E(8360)
    } else if (args.route == 2) {  // Malacký kanál
      A(9560)
      B
      F
      D
      E(9560)
    } else if (args.route == 3) {  // Sundský průliv
      A(10677) 
      G(10677) 
      H(10677) 
      D 
      E(10677)
    } else {  // Lombocký průliv
      A(12223) 
      G(12223) 
      I(12223) 
      E(12223)
    }
  JMP_TANKER_LOST:
    price = (km * args.fuel_cost);

    if (tanker_lost) {
      price = TANKER_PRICE;
      tankers_lost++;
    }

    if (args.verbose_output) {
      if (tanker_lost) {
        route_info_all << "TANKER LOST.\t" << price << "$\n";
      } else {
        route_info_all << i << ": " << km  << "km,\t"  << hours / 24  << "d "  << hours % 24  << "h,  \t"  << price  << "$\n";
      }
    }

    total_km += km;
    total_hours += hours;
    total_price += price;
  }
  avg_km = total_km / args.count;
  avg_hours = total_hours / args.count;
  avg_price = total_price / args.count;

  route_info_avg.open(args.filename_avg, ios::out);
  route_info_avg.precision(9);
  route_info_avg << "\t\tHodnoty Simulace " << args.filename_avg << ":\nprůměrně najeto: " << avg_km  << " kilometrů\nprůměrný čas plavby: " \
  << int(avg_hours / 24)  << "d "  << int(avg_hours) % 24  << "h\nCelkem zničeno tankerů: " << tankers_lost << " (";
  route_info_avg.precision(3);
  route_info_avg << 100 * ((float) tankers_lost / (float) args.count) << "%)\nPrůměrná cena plavby: ";
  route_info_avg.precision(9);
  route_info_avg << avg_price  << "$\n";
  

  route_info_avg.close();
  if (args.verbose_output) {
    route_info_all.close();
  }

  free(args.filename_avg);
  free(args.filename_all);

  exit(EXIT_SUCCESS);
}
