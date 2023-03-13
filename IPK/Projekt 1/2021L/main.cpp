// Samuel Kuchta (xkucht11)

#include "lib.hpp"

int main(int argc, char** argv) { // ./hinfosvc port
  if (argc != 2) {  // Parsing Argumentov
    USAGE(argv[0])  // nápoveda
  }
  char* end_ptr;  // ukazatel na retazec, ak port_num bude obsahovat pismena. (ak nenulovy, tak chyba).
  uint32_t port_num = strtoul(argv[1], &end_ptr, 10); // 12345
  if (*end_ptr || port_num > 0xFFFF) {  // cislo portu vacsie ako 65535, alebo text obsahoval pismená.
    cerr << "'" << argv[1] << "' neplatné číslo portu\n";
    USAGE(argv[0])  // nápoveda
  }

  Socket socket(port_num);
  if(socket.Socket_init()) {  // Vytvorenie a nastavenie pasívného TCP soketu
    return 99;
  } 
  int opt = 0;  // debugging
  while (socket.Socket_accept()) {  // můžete se socketem pracovat jako se souborem. do soketu processing_socket sa extrahuje 1. správa v poradí z pasívného soketu sock. 
    socket.Socket_read();
    Manage Manager = Manage(socket.processing_socket, socket.buf);  // trieda, ktora sa postara o zvysnu funkcionalitu volania prikazov.
    while (socket.Byte_cnt == SIZE) {  // Ak má správa viac ako 2^16 Bajtov, tak prečítaj dalšie Bajty, a ulož do buffer[].
      socket.Socket_read();  // Pokud tak ještě neučinils, vyčti zbytek dat v socketu ať si třeba prohlížeč nestěžuje
    }
    socket.Socket_terminate();  // zavri soket
  }
  opt++;  // debugging
  if (socket.processing_socket < 0) {  // accept vracia -1 pri chybe
    cerr << "Chyba Soketu: Selhal příjem ze soketu\n";
    return 99;
  }
  return 0;
}