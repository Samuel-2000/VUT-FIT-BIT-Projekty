# Dokumentace používání projektu Generování NetFlow dat ze zachycené síťové komunikace předmětu ISA 2022/2023
login: __xkucht11__
meno a priezvisko: __Samuel Kuchta__

funkcie programu: NetFlow exportér, který čte pakety, a odosílá NetFlow záznamy na Kolektor.

## použití:
v termináli, ve složce s projektem zadajte následující příkazy:
  make
  nfcapd -T all -l . -I any -p 63000
  ctrl+Z            # zastavenie serveru
  bg                # server beží na pozadí
  ./flow -f input.pcap -c 127.0.0.1:63000
  nfdump -r nfcapd.202211141115


### 1 Kolektor
Než spustíme náš exportér, je potřeba spustit kolektor, který bude zachytávat příchozí NetFlow záznamy.

#### 1.1 Spuštění Kolektoru:
nfcapd -T all -l . -I any -p <port>

#### 1.2 Příklady Spuštění:
nfcapd -T all -l . -I any -p 63000
nfcapd -T all -l . -I any -p 2055


### 2 Flow exporter
Teď spustíme náš program, který bude z hlaviček paketů generovat záznamy, a odesílat je na kolektor.

#### 2.1 Spuštění exporteru:
./flow [-f <file>] [-c <netflow_collector>[:<port>]] [-a <active_timer>] [-i <inactive_timer>] [-m <count>]
Kde:
-f <file> jméno analyzovaného souboru nebo STDIN,
-c <neflow_collector:port> IP adresa, nebo hostname NetFlow kolektoru. volitelně i UDP port (127.0.0.1:2055, pokud není specifikováno),
-a <active_timer> - interval v sekundách, po kterém se exportují aktivní záznamy na kolektor (60, pokud není specifikováno),
-i <seconds> - interval v sekundách, po jehož vypršení se exportují neaktivní záznamy na kolektor (10, pokud není specifikováno),
-m <count> - velikost flow-cache. Při dosažení max. velikosti dojde k exportu nejstaršího záznamu v cachi na kolektor (1024, pokud není specifikováno).
Všechny parametry jsou brány jako volitelné. Pokud některý z parametrů není uveden, použije se místo něj výchozí hodnota.
Příklad použití:

#### 2.2 Příklady Spuštění:
./flow
./flow -f input.pcap -c 127.0.0.1:63000


### 3 NFDUMP
Odchycené záznamy kolektorem chceme prohlížet pomocí programu nfdump.

#### 3.1 Spuštění NFDUMP:
nfdump -r <file>

#### 3.2 Příklady Spuštění:
nfdump -r nfcapd.202211141115
