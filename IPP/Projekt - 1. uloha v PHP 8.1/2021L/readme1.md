# Dokumentácia k 1. úlohe z predmetu IPP 2021/2022 
login: __xkucht11__
meno a priezvisko: __Samuel Kuchta__ 

## 1. Úvod
Zadanie bolo implementovať v jazyku PHP 8.1.2 skript parse.php, ktorý vykonává lexikálnu a syntaktickú analýzu jazyka IPPcode22. 

## 2. Implementácia
Ako pomocnú knižnicu som použil `DOMdocument`, ktorá pomáha vytvárať XML kód. 

### 2.1 I/O
Skript parse.php číta zdrojový kód jazyka IPPcode22 zo stdin.
Výstupom je XML reprezentácia programu, ktorá je vypísaná na stdout.
Chybové hlásenia sú vypisované na stderr.

### 2.2 Syntaktická analýza
Syntaktická analýza sa vykonáva vo funkcii `parser()` ktorá je implementovaná konečným automatom pomocou cyklu while a switchu.
funkcia `parser()` taktiež slůži na generovanie XML inštrukcií a jej podstromov.
`parser()` pre jednotlivé riadky kódu IPPcode22 volá lexikálnu analýzu pomocou funkcie `tokenize_next_line()`

### 2.3 Lexikálná analýza
Funkcia `tokenize_next_line()` vracia pole polí, kde jednotlivé polia tvoria inštrukcie a argumenty s pomocnými dátami.
Implementácia `tokenize_next_line()` je založená hlavne na využití regulárných výrazov.

#### 2.3.1 pole polí (priklady):
##### 2.3.1.1
  Token INST_NAME
  TOKEN_VAR/TOKEN_LABEL var/label
  TOKEN_VAR var
  TOKEN_VAR var

##### 2.3.1.2
  Token INST_NAME
  TOKEN_CONST type value
  TOKEN_VAR var

## 3. Spustenie
Na serveri Merlin v složke projektu, zadajte do terminálu príkaz php8.1 parse.php < 'vstup IPPcode22'
pre pomoc zadajte php8.1 parse.php --help
