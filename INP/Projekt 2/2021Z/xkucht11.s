; Vernamova sifra na architekture DLX
; Samuel Kuchta xkucht11

         .data 0x04          ; zacatek data segmentu v pameti
login:   .asciiz "xkucht11"
cipher:  .space 9	     ; sem ukladejte sifrovane znaky (za posledni nezapomente dat 0)
sude_k:  .byte 11	     ; sude  cislo (k) +
liche_u: .byte 21	     ; liche cislo (u) -
laddr:   .word login         ; 4B adresa vstupniho textu (pro vypis)
caddr:   .word cipher        ; 4B adresa sifrovaneho retezce (pro vypis)

        .text 0x40           ; adresa zacatku programu v pameti
        .global main        

main: 
  andi r23, r26, 1	     ; porovnej lichost
  seqi r1, r23, 1
  bnez r1, liche	     ; jestli liche, tak skoc.
  lbu r15, sude_k  	     ; r15 = 11 (k)
  j kontrola_preteceni
  nop

liche:
  lbu r15, liche_u            ; r15 = 21 (u)
  sub r15, r0, r15  	     ; r15 = -21

kontrola_preteceni:
  lbu r10, login(r26)	     ; nacti pismeno
  add r10, r10, r15	     ; pricti k nemu r15

  slei r1, r10, 96	     ; jestli je vysledek mensi nez a, tak jdi zpatky od z
  bnez r1, podteceni
	
  slti r1, r10, 123	     ; jestli je vysledek vetsi nez z, tak jdi zpatky od a
  bnez r1, pokracuj
  nop
  subi r10, r10, 26	     ; jestli je vysledek vetsi nez z, tak jdi zpatky od a
  j pokracuj
  nop

podteceni:
  addi r10, r10, 26	     ; jestli je vysledek mensi nez a, tak jdi zpatky od z

pokracuj:
  sb cipher(r26), r10	     ; uloz pismeno
  addi r26, r26, 1	     ; inkrementuj citac
  lbu r23, login(r26)

  slti r1, r23, 97   	     ; neni male pismeno = konec retazca, vyskakujem z cyklu
  bnez r1, end

  sgti r1, r23, 122           ; neni male pismeno = konec retazca, vyskakujem z cyklu
  bnez r1, end
  
  j main
  
end:
  sb cipher(r26), r0 	     ; 0 na konci
  addi r14, r0, caddr	     ; <-- pro vypis sifry nahradte laddr adresou caddr
  trap 5  		     ; vypis textoveho retezce (jeho adresa se ocekava v r14)
  trap 0  		     ; ukonceni simulace