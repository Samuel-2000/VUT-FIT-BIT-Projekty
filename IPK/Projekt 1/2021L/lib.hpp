// Samuel Kuchta (xkucht11)

#pragma once

#include <netinet/in.h>
#include <unistd.h>  // The pipe() function will create a pipe and place two file descriptors, one each into the arguments
#include <sys/socket.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <cstring> // similar to normal strings, but they contain an extra null character at the end of the string that makes them Cstrings.
#include <thread>
#include <chrono>

using namespace std;

  // makrá
#define SIZE 65536 // teoreticky staci aj 16, 256, 4096, ale prakticky moze nastat problem, ked pocet bajtov sprave je nasobkom size, takze treba taky velky priestor, ze sa tato sanca minimalizuje.

#define USAGE(_prog_name)                                                                                                                             \
  cerr << "Použítí: " << _prog_name << " port\nSpusťí lokální HTTP server na zadaném portu, který poskytuje základní systémové informace.\n\n";  \
  return 1;

#define CALL_FUN(_func, _err_str)                       \
  if (pack_and_ship(processing_socket, 200, _func())) { \
    ERR_INTERNAL(_err_str)                              \
  }

#define CALL_ERR(_func, _num)                                           \
  if (_func(processing_socket, _num, stat_code_dictionary.at(_num))) {  \
    ERR_INTERNAL2(_num, stat_code_dictionary.at(_num))                  \
  }

#define ERR_INTERNAL(_string)                                                         \
  cerr << "Interní Chyba: " << _string << ": Odeslání dat do soketu se nezdařilo\n";  

#define ERR_INTERNAL2(_num, _string)                                                                \
  cerr << "Interní Chyba: " << _num << " " << _string << ": Odeslání dat do soketu se nezdařilo\n";    

#define ERR_SOCKET(_string)                   \
  cerr << "Chyba Soketu " << _string << "\n"; \
  return 99;

// Triedy

class Socket {
  public:
    Socket(uint32_t port_num);
    int Socket_init();
    int Socket_accept();
    void Socket_read();
    ssize_t Socket_write();
    void Socket_terminate();

    int listening_socket, processing_socket; // deskriptory soketov
    alignas(SIZE) char buf[SIZE];  // buffer o 65536 znakoch, zarovnaných v pamäti.
    ssize_t Byte_cnt;  // počet prečítaných Bajtov (0-65536).
    int opt = 1;  // Nastavenie možností soketu
    sockaddr_in addr{AF_INET, 0, INADDR_ANY, 0};  // štruktúra adresy
    socklen_t addrLen = sizeof(addr);  // veľkosť štruktúry
};


class Manage {
  public:
    Manage(int processing_socket, string str_var);
    bool pack_and_ship(int sock, int status, const string &content);
    string hostname_fetch();
    string CPU_Name_fetch();
    string CPU_Load_fetch();

  private:
  // premenne/dátove typy
    int socket;
    const char* inst = "cat /proc/cpuinfo | grep 'model name' | head -n1 | awk -F':' '{ print $2 }'";
    const char* host_path = "/proc/sys/kernel/hostname";
    const char* cpu_stat = "/proc/stat";

    const map<int, string> stat_code_dictionary{{200, "OK"}, {400, "Bad Request"}, {500, "Internal Server Error"}};  // slovník, mapuje chybové kódy k retazcom.
    struct CPU_load_struct {  // informácie o stave procesora
      uint64_t user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    };

    CPU_load_struct fetch_cpu_stat();
};



