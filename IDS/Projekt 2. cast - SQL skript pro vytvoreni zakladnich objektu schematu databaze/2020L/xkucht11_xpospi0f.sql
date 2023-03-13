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
    rodne_cislo INT PRIMARY KEY,
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
    clen INT,
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
    VALUES(8752143214, 'Oliveira', 'Silene', 32, 'Podsef', 'Ozbrojene loupeze', 'Bozetechova 1', 'tokyo@lacasadepapel.es', 'Zachrana kolegy', 'Marquina');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES(7411101123, 'De Fonollosa', 'Andres', 43, 'Podsef', 'Kradeze sperku', 'Krkoskova 16', 'berlin@lacasadepapel.es', 'Plany loupeze', 'Cano');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES(9214113698, 'Jimenez', 'Agata', 33, 'Kapitan', 'Obchod s drogami', 'Erbenova 32', 'nairobi@lacasadepapel.es', '', 'Marquina');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES(5512107569, 'Ramos', 'Augustin', 55, 'Pesak', 'Specalista na mechanicke prace', 'Moskevska 12', 'moscow@lacasadepapel.es', '', 'Berrote');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES(6805107569, 'Dragic', 'Radko', 55, 'Pesak', 'Prace se zbranemi', 'Skandinavska 36', 'oslo@lacasadepapel.es', '', 'Dragic');
INSERT INTO radovy_clen(rodne_cislo, jmeno, prijmeni, vek, postaveni, specializace, adresa, emailova_adresa, zasluhy, rodina)
    VALUES(7452143528, 'Gaztambide', 'Monica', 27, 'Pesak', 'Ucetni', 'Masarykova 5', 'stockholm@lacasadepapel.es', '', 'Cano');   

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
    VALUES('zavrazdeni Vita Corleoneho', 5512107569, 2);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vyber vypalneho', 9214113698, 3);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', 8752143214, 3);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', 7411101123, 1);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', 9214113698, 1);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', 5512107569, 3);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', 6805107569, 2);
INSERT INTO provadi_cinnost(cinnost, clen, prostredek)
    VALUES('Vykradeni mincovny', 7452143528, 4);


    
  -- na 3. cvicenie
--SELECT * FROM don;
--SELECT * FROM aliance;
--SELECT * FROM uzemi;

--SELECT * FROM prislusnost_k_alianci;
--SELECT * FROM setkani_donu;
--SELECT * FROM ucastni_se_setkani;

--SELECT * FROM radovy_clen;
--SELECT * FROM cinnost;
--SELECT * FROM prostredky;
--SELECT * FROM vrazda;
--SELECT * FROM provadi_cinnost;