-- xpospi0f, xkucht11

  -- smazani tabulek
DROP TABLE don CASCADE CONSTRAINTS;
DROP TABLE aliance CASCADE CONSTRAINTS;
DROP TABLE uzemi CASCADE CONSTRAINTS;

DROP TABLE prislusnost_k_alianci CASCADE CONSTRAINTS;
DROP TABLE setkani_donu CASCADE CONSTRAINTS; 
DROP TABLE ucastni_se_setkani CASCADE CONSTRAINTS;

DROP TABLE radovy_clen CASCADE CONSTRAINTS; 
DROP TABLE cinnost CASCADE CONSTRAINTS;
DROP TABLE prostredky CASCADE CONSTRAINTS; 
DROP TABLE vrazda CASCADE CONSTRAINTS; 
DROP TABLE provadi_cinnost CASCADE CONSTRAINTS;




  -- vytvoreni tabulkek
CREATE TABLE don (
    rodina VARCHAR(255) PRIMARY KEY, --zaroven i prijmeni
    jmeno VARCHAR(255) NOT NULL,
    vek INT NOT NULL,
    velikost_bot INT NOT NULL
);

CREATE TABLE aliance (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    doba_trvani VARCHAR(255) NOT NULL,
    datum_vytvoreni DATE NOT NULL
);

CREATE TABLE uzemi (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    rajon VARCHAR(255) NOT NULL, -- uzemi vlastni DON 
    adresa VARCHAR(255) NOT NULL,
    souradnice VARCHAR(255) NOT NULL,
    rozloha VARCHAR(255) NOT NULL,
    rodina VARCHAR(255),
    
    CONSTRAINT fk_vlastnik FOREIGN KEY (rodina)
        REFERENCES don(rodina)     
);



CREATE TABLE prislusnost_k_alianci (
    aliance INT,
    rodina VARCHAR(255),
    
    PRIMARY KEY (aliance, rodina),
    
    CONSTRAINT fk_aliance FOREIGN KEY (aliance)
        REFERENCES aliance(id),     
    CONSTRAINT fk_rodina_prislusnost FOREIGN KEY (rodina)
        REFERENCES don(rodina)    
);

CREATE TABLE setkani_donu (
    datum DATE PRIMARY KEY,
    uzemi INT,
    
    CONSTRAINT fk_uzemi FOREIGN KEY (uzemi)
        REFERENCES uzemi(id)   
);

CREATE TABLE ucastni_se_setkani (
    don VARCHAR(255),
    setkani DATE,
    
    PRIMARY KEY (don, setkani),
    
    CONSTRAINT fk_zucastneny_don FOREIGN KEY (don)
        REFERENCES don(rodina),    
    CONSTRAINT fk_setkani FOREIGN KEY (setkani)
        REFERENCES setkani_donu(datum)
);



CREATE TABLE radovy_clen (
    rodne_cislo CHAR(11) PRIMARY KEY,
    jmeno VARCHAR(255) NOT NULL,
    prijmeni VARCHAR(255) NOT NULL, 
    vek INT NOT NULL,
    postaveni VARCHAR(255) NOT NULL,
    specializace VARCHAR(255) NOT NULL,
    adresa VARCHAR(255) NOT NULL,
    emailova_adresa VARCHAR(255) NOT NULL,
    zasluhy VARCHAR(255),
    rodina VARCHAR(255),
    
    CONSTRAINT fk_patri_pod FOREIGN KEY (rodina)
        REFERENCES don(rodina)
);

CREATE TABLE cinnost (
    nazev VARCHAR(255) PRIMARY KEY,
    druh VARCHAR(255) NOT NULL,
    uzemi_cinnosti INT NOT NULL, -- odehrava se na uzemi
    objednal VARCHAR(255) NOT NULL, -- don
    doba_trvani VARCHAR(255) NOT NULL,
    datum DATE NOT NULL,
    
    CONSTRAINT fk_uzemi_cinnosti FOREIGN KEY (uzemi_cinnosti)
        REFERENCES uzemi(id),
    CONSTRAINT fk_objednal FOREIGN KEY (objednal)
        REFERENCES don(rodina)
);

CREATE TABLE prostredky (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    druh VARCHAR(255) NOT NULL,
    nazev VARCHAR(255) NOT NULL,
    stav VARCHAR(255) NOT NULL,
    cena INT NOT NULL
);

CREATE TABLE vrazda (
    nazev VARCHAR(255) PRIMARY KEY,
    datum_vrazdy DATE,
    
    CONSTRAINT fk_vrazda_nazev FOREIGN KEY (nazev)
        REFERENCES cinnost(nazev)
);

CREATE TABLE provadi_cinnost(
    cinnost VARCHAR(255),
    clen CHAR(11),
    prostredek INT,
    
    PRIMARY KEY (cinnost, clen, prostredek),
    
    CONSTRAINT fk_provadi_cinn FOREIGN KEY (cinnost)
        REFERENCES cinnost(nazev),
    CONSTRAINT fk_prov_cinn_clen FOREIGN KEY (clen)
        REFERENCES radovy_clen(rodne_cislo),
    CONSTRAINT fk_prov_cinn_prostredek FOREIGN KEY (prostredek)
        REFERENCES prostredky(id)
);




    -- vkladani konkretnich dat do tabulek
  -- Donove
INSERT INTO don(rodina, jmeno, vek, velikost_bot)
    VALUES('Marquina', 'Sergio', 42, 43);
INSERT INTO don(rodina, jmeno, vek, velikost_bot)
    VALUES('Murillo', 'Raquel', 37, 38);
INSERT INTO don(rodina, jmeno, vek, velikost_bot)
    VALUES('Cano', 'Matias', 27, 44);    
INSERT INTO don(rodina, jmeno, vek, velikost_bot)
    VALUES('Berrote', 'Martin', 34, 42);    
INSERT INTO don(rodina, jmeno, vek, velikost_bot)
    VALUES('Dragic', 'Mirko', 27, 44);    
    
  -- Aliance
INSERT INTO aliance(doba_trvani, datum_vytvoreni)
    VALUES('157 dni', TO_DATE('4.8.2006', 'DD/MM/YYYY'));
INSERT INTO aliance(doba_trvani, datum_vytvoreni)
    VALUES('297 dni', TO_DATE('4.8.2007', 'DD/MM/YYYY'));
INSERT INTO aliance(doba_trvani, datum_vytvoreni)
    VALUES('1124 dni', TO_DATE('4.8.2002', 'DD/MM/YYYY'));

  -- Uzemi
INSERT INTO uzemi(rajon, adresa, souradnice, rozloha, rodina)
    VALUES('Dragic', 'Draci 69', '4.7265N 7.9815W', '12 km2', 'Marquina');
INSERT INTO uzemi(rajon, adresa, souradnice, rozloha, rodina)
    VALUES('Murillo', 'Nevimova 14', '14.7265N 17.9815W', '17 km2', 'Dragic');
INSERT INTO uzemi(rajon, adresa, souradnice, rozloha, rodina)
    VALUES('Berrote', 'Royal Mint', '22.1354N 17.9815W', '65 km2', 'Cano');



  -- Prislunosti k alianci
INSERT INTO prislusnost_k_alianci(aliance, rodina)
    VALUES(1, 'Marquina');
INSERT INTO prislusnost_k_alianci(aliance, rodina)
    VALUES(1, 'Berrote');
INSERT INTO prislusnost_k_alianci(aliance, rodina)
    VALUES(1, 'Dragic');   
INSERT INTO prislusnost_k_alianci(aliance, rodina)
    VALUES(2, 'Murillo');
INSERT INTO prislusnost_k_alianci(aliance, rodina)
    VALUES(2, 'Berrote');
INSERT INTO prislusnost_k_alianci(aliance, rodina)
    VALUES(2, 'Cano');   
INSERT INTO prislusnost_k_alianci(aliance, rodina)
    VALUES(3, 'Dragic');
INSERT INTO prislusnost_k_alianci(aliance, rodina)
    VALUES(3, 'Berrote');

  -- setkani donu
INSERT INTO setkani_donu(datum, uzemi)
    VALUES(TO_DATE('1/10/2015', 'DD/MM/YYYY'), 1);

  -- ucast donu na setkani
INSERT INTO ucastni_se_setkani(don, setkani)
    VALUES('Marquina', TO_DATE('1/10/2015', 'DD/MM/YYYY'));
INSERT INTO ucastni_se_setkani(don, setkani)
    VALUES('Murillo', TO_DATE('1/10/2015', 'DD/MM/YYYY'));
INSERT INTO ucastni_se_setkani(don, setkani)
    VALUES('Berrote', TO_DATE('1/10/2015', 'DD/MM/YYYY'));



  -- Dalsi clenove
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES('875214/3214', 'Oliveira', 'Silene', 32, 'Podsef', 'Ozbrojene loupeze', 'Bozetechova 1', 'tokyo@lacasadepapel.es', 'Zachrana kolegy', 'Marquina');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES('741110/1123', 'De Fonollosa', 'Andres', 43, 'Podsef', 'Kradeze sperku', 'Krkoskova 16', 'berlin@lacasadepapel.es', 'Plany loupeze', 'Cano');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES('921411/3698', 'Jimenez', 'Agata', 33, 'Kapitan', 'Obchod s drogami', 'Erbenova 32', 'nairobi@lacasadepapel.es', '', 'Marquina');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES('551210/9765', 'Ramos', 'Augustin', 55, 'Pesak', 'Specalista na mechanicke prace', 'Moskevska 12', 'moscow@lacasadepapel.es', '', 'Berrote');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES('680510/7569', 'Dragic', 'Radko', 55, 'Pesak', 'Prace se zbranemi', 'Skandinavska 36', 'oslo@lacasadepapel.es', '', 'Dragic');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES('745214/3528', 'Gaztambide', 'Monica', 27, 'Pesak', 'Ucetni', 'Masarykova 5', 'stockholm@lacasadepapel.es', '', 'Cano');   
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES('395714/4798', 'Giuseppe', 'Lenouch', 27, 'Novacek', 'vybusniny', 'Berkova 13', 'lapizza@lacasadepapel.es', '', 'Berrote');
    
    
    
  -- Cinnost
INSERT INTO cinnost(nazev, druh, uzemi_cinnosti, objednal, doba_trvani, datum)
    VALUES('zavrazdeni Vita Corleoneho', 'Vrazda', 1, 'Dragic', '30 minut', TO_DATE('10.6.2020', 'DD/MM/YYYY'));
INSERT INTO cinnost(nazev, druh, uzemi_cinnosti, objednal, doba_trvani, datum)
    VALUES('Vyber vypalneho', 'Vydirani', 2, 'Murillo', '25 minut', TO_DATE('11.6.2020', 'DD/MM/YYYY'));
INSERT INTO cinnost(nazev, druh, uzemi_cinnosti, objednal, doba_trvani, datum)
    VALUES('Vykradeni mincovny', 'Vloupani', 3, 'Marquina', '14 dni', TO_DATE('12.6.2020', 'DD/MM/YYYY'));
INSERT INTO cinnost(nazev, druh, uzemi_cinnosti, objednal, doba_trvani, datum)
    VALUES('Unos', 'Vydirani', 2, 'Murillo', '1 hodina', TO_DATE('22.6.2020', 'DD/MM/YYYY'));

  -- prostredky
INSERT INTO prostredky(druh, nazev, stav, cena)
    VALUES('Naradi', 'Kladivo', 'Dobry', 400);
INSERT INTO prostredky(druh, nazev, stav, cena)
    VALUES('Naradi', 'Krumpac', 'Dobry', 800);
INSERT INTO prostredky(druh, nazev, stav, cena)
    VALUES('Zbrane', 'Browning Machine Gun', 'Dobry', 1500000);
INSERT INTO prostredky(druh, nazev, stav, cena)
    VALUES('Zbrane', 'Revolver', 'Spatny', 25000);

  -- Vrazda
INSERT INTO vrazda(nazev, datum_vrazdy)
    VALUES('zavrazdeni Vita Corleoneho', TO_DATE('1/10/2016', 'DD/MM/YYYY'));

  -- Provadi cinnost
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('zavrazdeni Vita Corleoneho', '551210/9765', 1);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vyber vypalneho', '921411/3698', 4);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', '875214/3214', 4);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', '741110/1123', 1);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', '921411/3698', 2);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', '551210/9765', 3);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', '680510/7569', 2);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', '745214/3528', 4);


    

  -- SELECT
SELECT * FROM don;
SELECT * FROM aliance;
SELECT * FROM uzemi;

SELECT * FROM prislusnost_k_alianci;
SELECT * FROM setkani_donu;
SELECT * FROM ucastni_se_setkani;

SELECT * FROM radovy_clen;
SELECT * FROM cinnost;
SELECT * FROM prostredky;
SELECT * FROM vrazda;
SELECT * FROM provadi_cinnost;




    -- SQL skript obsahující dotazy SELECT musí obsahovat konkrétně alespoň dva dotazy využívající spojení dvou tabulek
    
  -- v kterych cinnostech se pouzivaji zbrane
SELECT DISTINCT C.cinnost
FROM provadi_cinnost C, prostredky P
WHERE C.prostredek=P.id AND P.druh='Zbrane';

  -- na jakych adresach se budou odehravat konkretni setkani donu
SELECT S.datum, U.adresa
FROM setkani_donu S, uzemi U
WHERE S.uzemi=U.id;



    -- jeden využívající spojení tří tabulek

  -- ktorych radovych clenov ma k dispozicii 1. aliancia
SELECT D.rodina, C.rodne_cislo, C.jmeno, C.prijmeni
FROM prislusnost_k_alianci P, don D, radovy_clen C
WHERE P.rodina=D.rodina AND D.rodina=C.rodina AND P.aliance=1;



    -- dva dotazy s klauzulí GROUP BY a agregační funkcí

  -- prumerny vek clenu konkretnich rodin
SELECT C.rodina, AVG(C.vek) AS "prumerny_vek_clenu"
FROM radovy_clen C
GROUP BY C.rodina;

  -- prumerna a celkova cena prostredku
SELECT P.druh, AVG(P.cena) AS "prumer", Sum(P.cena) AS "celkem"
FROM prostredky P
GROUP BY P.druh;



    -- jeden dotaz obsahující predikát EXISTS

  -- ktery radovy clenove nemaji pridelenou cinnost
SELECT R.rodne_cislo, R.jmeno, R.prijmeni
FROM radovy_clen R
WHERE NOT EXISTS (
     SELECT * 
     FROM provadi_cinnost P
     WHERE R.rodne_cislo=P.clen
    );



    -- jeden dotaz s predikátem IN s vnořeným selectem (nikoliv IN s množinou konstantních dat).
  -- zjisti, ktery clenove pouzivaji lacne prostredky na nektere cinnosti (nedostatek kvalitniho vybaveni -> treba dokoupit)
  -- ktery radovy clenove nepouzivaji zadny prostredek ze seznamu pri nejake z cinnosti
  -- (musi delat alespon 1 cinnost, a pri nektere z nich nesmi pouzivat ani jeden prostredek z konkretniho seznamu prostredku (s cenou nad 5000)).
SELECT R.rodne_cislo, R.jmeno, R.prijmeni, PC.cinnost, P.nazev, P.cena
FROM radovy_clen R, prostredky P, provadi_cinnost PC
WHERE R.rodne_cislo=PC.clen AND PC.prostredek=P.id 
     AND PC.prostredek=P.id AND P.cena NOT IN (
	 SELECT P.cena
	 FROM prostredky P
     WHERE P.cena > 5000
    );