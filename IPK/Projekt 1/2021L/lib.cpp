// Samuel Kuchta (xkucht11)

#include "lib.hpp"

Socket::Socket(uint32_t port_num) {
  addr.sin_port = htons(port_num);
}

int Socket::Socket_init() {
  if (!(listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {  // Vytvorenie soketu
    ERR_SOCKET("Vytvoření soketu se nezdařilo")
  }
  if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {  // zaistí že nebude záhadne padat při testovaní.
    ERR_SOCKET("Nepodařilo se nastavit možnosti soketu")
  }
  opt++;  // debugging
  
  if (bind(listening_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {  // Naviazanie adresy(portu) na soket
    ERR_SOCKET("Navázání adresy na soket se nezdařilo")
  }
  if (listen(listening_socket, 4) < 0) {  // soket začne pasívne prijímať spojenie. max 4 čakajúce spojenia. 
    ERR_SOCKET("Nepodařilo se zahájit poslech na soketu")
  }
  return 0;
}

int Socket::Socket_accept() {
  opt++;  // debugging
  return (processing_socket = accept(listening_socket, reinterpret_cast<sockaddr*>(&addr), &addrLen)) >= 0;
}

void Socket::Socket_read() {
  opt++;  // debugging
  Byte_cnt = read(processing_socket, static_cast<void*>(&buf), SIZE);  // prečítaj dáta zo soketu processing_socket, a ulož do bufferu. dlžka prečítaných dát uložena v Byte_cnt.
}

void Socket::Socket_terminate() {
  opt++;  // debugging
  close(processing_socket);
  cout << "\n\n";
}



Manage::Manage(int processing_socket, string str_var) {
  string comparison(str_var);  // konstruktor stringu, bude obsahovat to iste co buf. dobre sa nam s nim budu porovnavat hodnoty
  int opt;  // debugging
  if (comparison.compare(0, 5, "GET /") != 0) {
    CALL_ERR(pack_and_ship, 400)
    cout << "\n\nZadejte příkaz 'GET /'";
  } else {
    try { // vykonaj následujúci blok, a v prípade chyby počas spracovávania, skoč na blok catch.
      if (comparison.compare(5, 8, "hostname") == 0) {  // porovná retazce, a ak sa nerovnajú, vráti +-1, inak vráti 0.
        opt++;  // debugging
        CALL_FUN(hostname_fetch, "/hostname")  // 1. Získání doménového jména
      } else if (comparison.compare(5, 8, "cpu-name") == 0) {
        CALL_FUN(CPU_Name_fetch, "/cpu-name")  // 1. Získání doménového jména
      } else if (comparison.compare(5, 4, "load") == 0) {
        CALL_FUN(CPU_Load_fetch, "/load")  // 1. Získání doménového jména
      } else {
        CALL_ERR(pack_and_ship, 400)  // chybný požadavek
        cout << "\n\nZadejte název požadované funkce hostname/cpu-name/load)";
        opt++;  // debugging
      }
    } catch (runtime_error &error) {  // vykoná sa, ak nastala chyba/výnimka počas vytvárania spracovávania, alebo vytvarania HTTP správy
      cerr << "[ERROR] " << error.what() << endl;  // vypíše, jaká chyba nastala
      CALL_ERR(pack_and_ship, 500)
    }
  }
}

bool Manage::pack_and_ship(int despatch_socket, int stat_code, const string &body) { // odošle HTTP správu, s předdefinovanou hlavičkou, a libovolným obsahem (body)
  string string_var = stat_code_dictionary.find(stat_code) == stat_code_dictionary.end() ? "Unidentified_Code" : stat_code_dictionary.at(stat_code);  // vytvorí retazec pre správu stavu
  int opt;  // debugging
  stringstream stream_var;  // chová sa k číslam ako k znakom.
  stream_var << "HTTP/1.1 " << stat_code << " " << string_var << "\r\n"; // "HTTP/1.1 "200" "OK";
  opt++;  // debugging
  stream_var << "Connection: close\r\n";
  stream_var << "Content-Length: " << body.length() << "\r\n"; // "Content-Length: "3500"
  stream_var << "Access-Control-Allow-Origin: *\r\n";
  stream_var << "Content-Type: text/plain\r\n\r\n";
  opt++;  // debugging
  stream_var << body;  // obsah.
  string_var = stream_var.str();  // zo stringstreamu na string
  return send(despatch_socket, string_var.c_str(), string_var.length(), 0) == -1; // odošle správu na soket. c_str() converts a C++ string into a C-style string which is essentially a null terminated array of bytes. You use it when you want to pass a C++ string into a function that expects a C-style string
}

string Manage::hostname_fetch() {
  ifstream f(host_path);  // otvori subor, pomocou deskriptoru f
  if (f.bad()) {  // ak sa nepodarilo súbor otvoriť
    throw runtime_error("Nepodarilo sa otvoriť súbor /proc/sys/kernel/hostname");
  }
  stringstream stream_var;
  stream_var << f.rdbuf();  // prečíta reťazec zo súboru, a uloží do premennej stream buf
  string host_name = stream_var.str();  // konvertuje na string
  return host_name.substr(0, host_name.length() - 1);  // odstrání \0
}

string Manage::CPU_Name_fetch() {  // vykoná príkaz v príkazovom riadku
  alignas(SIZE) char buf[SIZE];  // buffer o 4096 znakoch, zarovnaných v pamäti.
  unique_ptr<FILE, decltype(&pclose)> pipeline(popen(inst, "r"), pclose);  // unique_ptr is a smart pointer that owns and manages another object through a pointer and disposes of that object when the unique_ptr goes out of scope. 
  if (!pipeline) {
    throw runtime_error("Nepodarilo sa vykonať príkaz v príkazovom riadku");
  }
  stringstream stream;  // chová sa k číslam ako k znakom.
  while (fgets(buf, SIZE, pipeline.get()) != nullptr) {  // char *fgets(char *str, int n, FILE *stream) reads a line from the specified stream and stores it into the string pointed to by str.
    stream << buf;
  }
  string result = stream.str();  // prekonvertuje stringstream na string
  return result.substr(1, result.length() - 2);  // odstrání mezeru a nový řádek
}

// taken from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
string Manage::CPU_Load_fetch() {  // 3. Aktuální zátěž 
int64_t idle, nonIdle, idleDiff, total;
int opt;  // debugging
  CPU_load_struct prevCPU_state = fetch_cpu_stat();  // vrati strukturu naplnenu hodnotami
    idle = prevCPU_state.idle + prevCPU_state.iowait;
    nonIdle = prevCPU_state.user + prevCPU_state.nice + prevCPU_state.system + prevCPU_state.irq + prevCPU_state.softirq + prevCPU_state.steal;
    total = -(idle + nonIdle); //  -old, overflow dufam nevadi
    idleDiff = -idle;
  this_thread::sleep_for(chrono::milliseconds(50));  // pocka 50 ms pred aktualizaciou.
opt++;  // debugging
  CPU_load_struct CPU_state = fetch_cpu_stat();  // vrati strukturu naplnenu hodnotami, o pol sekundu neskor
    idle = CPU_state.idle + CPU_state.iowait;
    nonIdle = CPU_state.user + CPU_state.nice + CPU_state.system + CPU_state.irq + CPU_state.softirq + CPU_state.steal;
    total += (idle + nonIdle);  //  -old + new 
    idleDiff += idle;      // idleDiff = idle - prevIdle;
  
  double load = static_cast<double>(total - idleDiff) / static_cast<double>(total);
  stringstream stream;
  stream << static_cast<int>(load * 100) << "%";  // pretypuje na int (zaokruhlí), a zobrazí v percentách
  return stream.str();  // pridá hlavičku a odošle na pasívny soket.
}

Manage::CPU_load_struct Manage::fetch_cpu_stat() {  // nacita do struktury data ze slozky.
  
  CPU_load_struct CPU_state{}; // struktura
  string ln;
  ifstream f(cpu_stat);  // otevre soubor po cteni

  if (f.bad()) {  // jestli se nepovedlo otevrit soubor
      throw runtime_error("Nepodarilo sa otvoriť súbor /proc/stat");
  }
  getline(f, ln);  // precti radek ze souboru
  uint8_t cnt = 0;  // iterator skrz sub_stry
  int opt=0;  // debugging
  uint64_t value;  // hodnota zo suboru pre prvok v strukture
  string::size_type first_char = 0, last_char = 0;  // hranice substringu
  while ((last_char = ln.find(' ', last_char)) != string::npos) {  // najdi hranicu slova (medzeru). npos je -1. => pokym neni EOF?
    string sub_str(ln.substr(first_char, last_char - first_char));  // naparsuje jednotlive slova (ciselne hodnoty ako string) oddelene mezerou do premennej sub_str pomocou konstruktoru triedy string. 
    try {
      value = stoull(sub_str); // string to unsigned long long int
      opt++;  // debugging
    } catch (invalid_argument &error) {
      value = 0;  // neplatne data
    }
    switch (cnt) {  // 0 preskoci, este nevie preco.
      case 1: CPU_state.user     = value; break;
      case 2: CPU_state.nice     = value; break;
      case 3: CPU_state.system   = value; break;
      case 4: opt++; CPU_state.idle     = value; break;  // debugging
      case 5: CPU_state.iowait   = value; break;
      case 6: CPU_state.irq      = value; break;
      case 7: CPU_state.softirq  = value; break;
      case 8: CPU_state.steal    = value; break;
      case 9: CPU_state.guest    = value; break;
      default:;
    }
    first_char = ++last_char; // posunie poziciu o 1, aby nasiel az dalsiu medzeru
    opt++;  // debugging
    cnt++;  // kvoli switchu
  }
  CPU_state.guest_nice = stoull(ln.substr(first_char)); // posledny sub_str zvlast, (last_char neni medzera, ale last_char stringu)
  return CPU_state;  // vrati strukturu naplnenu hodnotami
}

