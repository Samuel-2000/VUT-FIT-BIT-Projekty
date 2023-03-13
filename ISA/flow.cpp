// Samuel Kuchta (xkucht11)
// NetFlow v5 exporter ISA projekt 2022

#include "lib.hpp"

string collector_dst_ip;
string collector_dst_port;
int pocet = 0;
NF5_H flow_hdr;
NF5_F** pole_F;
uint32_t count = 1024;  // velikost flow-cache. Při dosažení max. velikosti dojde k exportu nejstaršího záznamu v cachi na kolektor
uint32_t active_timer = 60 * 1000000;  // interval v sekundách, po kterém se exportují aktivní záznamy na kolektor
uint32_t seconds = 10 * 1000000;  // interval v sekundách, po jehož vypršení se exportují neaktivní záznamy na kolektor
uint32_t boot_cas;
uint32_t boot_mikro_cas;


int main(int argc, char** argv) {  // ./flow [-f <file>] [-c <netflow_collector>[:<port>]] [-a <active_timer>] [-i <inactive_timer>] [-m <count>]
  // ArgParse-Start//
  if (argc % 2 != 1) {
    USAGE(argv[0]);  // nápoveda
    return 1;
  }
  char* end_ptr;
  string file = "-"; // jméno analyzovaného souboru nebo STDIN (-)
  string neflow_collector_destination = "127.0.0.1:2055";  // IP adresa, nebo hostname NetFlow kolektoru. volitelně i UDP port

  try {
    for (int i = 1; i < argc; i = i + 2) {
      if (strcmp(argv[i], "-f") == 0) {
        file = argv[i+1];
      } else if (strcmp(argv[i], "-c") == 0) {
        neflow_collector_destination = argv[i+1];
      } else if (strcmp(argv[i], "-a") == 0) {
        active_timer = strtoul(argv[i+1], &end_ptr, 10) * 1000000; // max -> 4,294,967,295
        if (*end_ptr) {
          cerr << "'" << argv[i+1] << "' je neplatné číslo intervalu exportu aktivnych zaznamov.\n";
          USAGE(argv[0])  // nápoveda
        }
      } else if (strcmp(argv[i], "-i") == 0) {
        seconds = strtoul(argv[i+1], &end_ptr, 10) * 1000000; // max -> 4,294,967,295
        if (*end_ptr) {
          cerr << "'" << argv[i+1] << "' je neplatné číslo intervalu exportu neaktivnych zaznamov.\n";
          USAGE(argv[0])  // nápoveda
        }
      } else if (strcmp(argv[i], "-m") == 0) {
        count = strtoul(argv[i+1], &end_ptr, 10); // max -> 4,294,967,295
        if (*end_ptr) {
          cerr << "'" << argv[i+1] << "' je neplatné číslo max velkosti flow cache.\n";
          USAGE(argv[0])  // nápoveda
        }
      } else {
        USAGE(argv[0]);  // nápoveda
        return 1;
      }
    }
  } catch (runtime_error &error) {  // vykoná sa, ak nastala chyba/výnimka počas parsovania argumentov
    cerr << "[ERROR] " << error.what() << endl;  // vypíše, aká chyba nastala
  }

  uint8_t length = neflow_collector_destination.length();
  int column_index;

  for (int i = 0; i < length; i++) {
    char znak = neflow_collector_destination[i];
    if (znak == ':') {
      column_index = i;
      break;
    }
    collector_dst_ip.push_back(neflow_collector_destination[i]);
  }

  //if (ip == true && column_index != -1) {
  for (int i = column_index + 1; i < length; i++) {
    collector_dst_port.push_back(neflow_collector_destination[i]);
  }
  // ArgParse-End //


  pcap_t* pcap_popisovac;
  char errbuf[PCAP_ERRBUF_SIZE];  // chyba pre pcap_functions .
  struct bpf_program bpf;  // štruktúra na filtrovanie .
  uint32_t maska;

  if ((pcap_popisovac = pcap_open_offline(file.c_str(), errbuf)) == NULL) {
    printf("pcap_open_offline(): %s\n", errbuf);
    return 1;
  }
  if (pcap_compile(pcap_popisovac, &bpf, "tcp or udp or icmp", 0, maska)) {  // prevedie výraz filtra na kód paketového filtra.
    printf("pcap_compile(): %s\n", pcap_geterr(pcap_popisovac));
    return 1;
  }
  if (pcap_setfilter(pcap_popisovac, &bpf) < 0) {  // priradí paketový filter k socketu
    printf("pcap_setfilter(): %s\n", pcap_geterr(pcap_popisovac));
    return 1;
  }

  flow_hdr.version = htons(5);  // htons((uint16_t)Num
  flow_hdr.count = htons(1);
  flow_hdr.flow_sequence = 0;
  flow_hdr.engine_type = 0;
  flow_hdr.engine_id = 0;
  flow_hdr.sampling_interval = 0;
  
  pole_F = (NF5_F**)calloc(count, sizeof(NF5_F*));
  for (int i = 0; i < count; i++) {
    pole_F[i] = (NF5_F*)calloc(1, sizeof(NF5_F));

  }

  if (pcap_loop(pcap_popisovac, 0, (pcap_handler) netFlow_analyze, 0) < 0) {  // vytiahne 1 paket, a preda ho funkcii netflow 
    printf("pcap_loop() zlyhalo: %s\n", pcap_geterr(pcap_popisovac));
  }

  pcap_close(pcap_popisovac);  // zavri deskriptor pcap.
  check_remaining(); // uisti sa, ze boli exportovane vsetky data.

  exit(EXIT_SUCCESS);
}
