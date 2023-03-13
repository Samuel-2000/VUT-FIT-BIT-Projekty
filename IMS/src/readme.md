# Dokumentace používání projektu Simulace Námořní dopravy předmětu IMS 2022/2023
login: __xkucht11__
jméno a příjmení: __Samuel Kuchta__

# 1 Funkce programu
Simulace Námořní dopravy.
z výsledků zjistíme, jakou trasou se vyplatí plout, při jakých koeficientech/situacích.

## 1.1 Spuštění simulace:
v termináli, ve složce s projektem zadajte následující příkazy:
- make
- ./sim

Program funguje na serveru Merlin. na Evě nefunguje překlad ve fázi linkování pravděpodobne z důvodu nekompatibility c++ knihovny fstream.

## 1.2 Vstupní parametry:
-t <route_set> - Vybere trasu kterou chceme odsimulovat.
- 1: Thajský kanál
- 2: Malacký kanál (výchozí)
- 3: Sundský průliv
- 4: Lombocký průliv

-c <count_set> - Nastaví, kolik krát se má trasa odsimulovat.
- min: 1
- výchozí: 10,000,000
- max: 1,000,000,000

-r <fuel_cost> - Cena ropy za barel v dolarech.
- min: 0
- výchozí: 80
- max: 1000

-w <wind_coef> - Koeficient výskytů hurikánů. (max 1500) \
-s <tsunami_coef> -  Koeficient výskytů tsunami. (max 8000) \
-k <haze_coef> -  Koeficient výskytů kouře/hmly. (max 5) \
-p <pirate_coef> -  Koeficient výskytů pirátů. (max 3000)
- 0: vypnuto, 
- (0, 1): menší šance
- 1: Normální (výchozí)
- (1-x): větší šance

-v <verbose_output> Ukládat do souboru i hodnoty jednotlivých plaveb.
- 0: vypnuto (výchozí)
- 1: zapnuto

-o <file_name> Explicitní název výstupního souboru.
- výchozí: ukládá podle příkladu v sekci 3.2

Všechny parametry jsou brány jako volitelné. Pokud některý z parametrů není uveden, použije se místo něj výchozí hodnota.

## 1.3 Příklady Spuštění:
- ./sim
- ./sim -t 1 -c 1000 -p 3
- ./sim -t 1 -c 1000 -p 3 -o myTest -v 1


# 2 Výstupy
Výsledky ukazují:
- cenu paliva,
- vzdálenost plavby,
- délka plavby,
- počet zničených tankerů,

# 2.1 Soubory
- vstupni-parametry_AVG.txt
- vstupni-parametry_ALL.txt (optional -v)

## 2.1.1 Příklad výstupních souborů:
- t1_c1000_p3__AVG.txt
- myTest__AVG.txt
- myTest__ALL.txt


