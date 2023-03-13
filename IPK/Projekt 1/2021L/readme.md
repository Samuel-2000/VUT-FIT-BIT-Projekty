IPK-Projekt-1 (11.3.2022)

Samuel Kuchta (xkucht11)

funkcia programu: server, z ktorého si pomocou http príkazu GET môžete vypítať informácie o jeho technických parametroch.

pouzitie:
v termináli, v složke s projektom zadajte následujuce príkazy:
  make
  ./hinfosvc 20000  # port je byt lubovolne cislo od 0 do 65535
  ctrl+Z            # zastavenie serveru
  bg                # server beží na pozadí

2 sposoby, ako si vyžiadať od serveru informacie:
1.
  curl http://localhost:20000/hostname
  curl http://localhost:20000/cpu-name
  curl http://localhost:20000/load

2.
telnet localhost 20000
GET /hostname
telnet localhost 20000
GET /cpu-name
telnet localhost 20000
GET /load


program na konci 2x odriadkuje \n, kvoli prehladnosti v terminali.


pri tvorbe kodu /load som sa inspiroval na: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux