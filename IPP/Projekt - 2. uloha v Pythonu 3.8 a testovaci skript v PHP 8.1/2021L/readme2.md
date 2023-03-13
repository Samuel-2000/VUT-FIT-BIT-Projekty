# Dokumentácia k 2. úlohe z predmetu IPP 2021/2022 
login: __xkucht11__
meno a priezvisko: __Samuel Kuchta__ 

## 1. Úvod
Zadanie bolo naimplementovať v jazyku Python 3.8 skript `interpret.py` a v jazyku PHP 8.1 skript `test.php`.

### `interpret.py`
analyzuje a interpretuje XML súbor s programom IPPCode22.

### `test.php`
testuje oba predchádzajúce skripty a generuje výstup vo formáte HTML5.

## 2. Implementácia `interpret.py`
Ako pomocnú knižnicu som použil `xml.etree.ElementTree`, ktorá pomáha spracovávať XML kód. 

### 2.1 I/O
Skript `interpret.py` číta vstup buď zo súboru, alebo zo štandardného vstupu. Výstupom je spracovanie samotného interpretovaného programu, prípadne jeho vypísanie na štandardný výstup. Chybové hlásenia sú vypisované na štandardný chybový výstup.

### 2.2 Hlavný tok
Hlavné telo skriptu sa nachádza v súbore `interpret.py`.

V prvom rade trieda `ArgsChecker` skontroluje argumenty skriptu. Zaistí správne priradenie hodnôt zadaných argumentmi programu, poprípade výpis pomocného textu.

Následne je nutné skontrolovať a získať dáta z XML reprezentácie programu, ktorý má byť interpretovaný. Tento problém rieši trieda `XmlParser`, ktorá s pomocou troch funkcií skontroluje integritu XML súboru a tiež inštrukcií v ňom obsiahnutých.
S pomocou funkcie `import_instructions()` sú tieto inštrukcie importované do zoznamu inštrukcií, aby mohli byť ďalej použité.

Samotná interpretácia prebieha formou (ne)konečného cyklu, kedy pri každom prechode je spracovaná jedna inštrukcia zo zoznamu inštrukcií.
Sú použité triedy:
`InstListCls` pre prácu so zoznamom inštrukcií a pre zistenie nasledujúcej inštrukcie.
`InstCls` pre uchovanie dát inštrukcie v štruktúrovanej podobe.`FrameCls` pre prácu s rámcom.

Pre uchovanie dát v datovom zásobníku je použitý zoznam `dataStack`.

Cyklus interpretácie končí buď chybou s jej adekvátným výpisom, alebo vyčerpaním listu inštrukcií a následným ukončením s návratovým kódom 0.

## 3. Implementácia skriptu `test.php`
### 3.1 I/O
Skript `test.php` hľadá testové súbory buď v aktuálnom adresári, alebo v zadanom adresári a vykoná ich otestovanie. Výstupom je prehľadná tabuľka vo formáte HTML5, ktorá je vypísaná na štandardný výstup. Chybové hlásenia sú vypisované na štandardný chybový výstup.

### 3.2 Hlavný tok
Hlavné telo skriptu sa nachádza v súbore `test.php`, ktorý zaisťuje samotné testovanie a uchovanie výsledkov testu.

`args_checker.php` slúži na kontrolu argumentov programu a priradenie ciest k súborom.

`file_manager.php` vyhľadáva testovacie položky a prípadne vytvára chýbajúce súbory.

`html_generator.php` generuje výsledný HTML súbor.

Najprv sa overia argumenty programu a zmapujú sa testové súbory `.src` v zadanom adresári. K týmto súborom sa prípadne vytvoria prázdne prídavné súbory `.in` a `.out`. Súbor `.rc` s návratovou hodnotou skriptu sa prípadne vytvorí s hodnotou `0`.

Následne sa pokračuje samotným testovaním. Najprv sa testuje skript `parse.php`. Na základe návratového kódu sa následne testuje interpretácia skriptom `interpret.py`.
Výsledky sa hneď vyhodnotia a uložia sa na ďalšie spracovanie.
Po dokončení testovania sa vygeneruje výsledná HTML stránka s prehľadným zobrazením výsledkov testov.