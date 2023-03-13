-- xpospi0f, xkucht11

-- SQL skript pro vytvoření pokročilých objektů schématu databáze.
-- Zadání č. 64 – Mafie (IUS 2019/2020).



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
  rodina VARCHAR(255) PRIMARY KEY,  -- zaroven i prijmeni
  jmeno VARCHAR(255) NOT NULL,
  datum_narozeni VARCHAR(255) NOT NULL, CHECK(REGEXP_LIKE(datum_narozeni, '[0-9]{2}(-)[0-9]{2}(-)[0-9]{4}')), -- puvodne vek. nepouzili jsme typ DATE, kvuli pouziti triggeru.
  velikost_bot INT NOT NULL
);

CREATE TABLE aliance (
  id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  doba_trvani VARCHAR(255) NOT NULL,
  datum_vytvoreni DATE NOT NULL
);

CREATE TABLE uzemi (
  id INT DEFAULT NULL PRIMARY KEY,  -- id generované skrz trigger.
  rajon VARCHAR(255) NOT NULL,  -- uzemi vlastni DON.
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




    -- triggery
  -- (1) Trigger pro automatické generování hodnot primárního klíče tabulky don ze sekvence.
DROP SEQUENCE ID_Uzemi;
CREATE SEQUENCE ID_Uzemi;

CREATE OR REPLACE TRIGGER ID_Uzemi 
  BEFORE INSERT ON uzemi 
  FOR EACH ROW
BEGIN
  IF :NEW.id IS NULL THEN
    :NEW.id := ID_Uzemi.NEXTVAL;
  END IF;
END;
/

  -- (2) Trigger pro Validaci data narozeni dona.
CREATE OR REPLACE TRIGGER Don_Datum_Narozeni BEFORE INSERT OR UPDATE OF datum_narozeni ON don FOR EACH ROW
BEGIN

  IF((SUBSTR(:NEW.datum_narozeni, 4, 2) IN ('01', '03', '05', '07', '08', '10', '12'))) THEN      -- if months that have 31 days
    IF NOT ((SUBSTR(:NEW.datum_narozeni, 1, 2) IN ('01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31'))) THEN
      RAISE_APPLICATION_ERROR(-20000, 'v zadanom mesiaci moze byt najviac 31 dni');
    END IF;

  ELSIF ((SUBSTR(:NEW.datum_narozeni, 4, 2) IN ('04', '06', '09', '11'))) THEN             -- if months that have 30 days
    IF NOT ((SUBSTR(:NEW.datum_narozeni, 1, 2) IN ('01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31'))) THEN
      RAISE_APPLICATION_ERROR(-20000, 'v zadanom mesiac imoze byt najviac 30 dni');
    END IF;

  ELSIF ((SUBSTR(:NEW.datum_narozeni, 4, 2) IN ('02'))) THEN                            -- if february (29 always passes)
    IF NOT ((SUBSTR(:NEW.datum_narozeni, 1, 2) IN ('01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29'))) THEN
      RAISE_APPLICATION_ERROR(-20000, 'v zadanom mesiaci moze byt najviac 29 dni');
    END IF;
  ELSE 
    RAISE_APPLICATION_ERROR(-20002, 'mesiac neni v rozmedzi 1-12');
  END IF;

  IF (CAST(SUBSTR(:NEW.datum_narozeni, 7, 4) AS int) > 2001) THEN
    RAISE_APPLICATION_ERROR(-20003, 'clen mafie musi dovrsit dospelost');
  ELSE
    IF (CAST(SUBSTR(:NEW.datum_narozeni, 7, 4) AS int) < 1900) THEN
      RAISE_APPLICATION_ERROR(-20003, 'tak stary clovek uz nezije');
    END IF;
  END IF;
END;
/
  -- puvodne melo podporovat i prestupne roky, ale nedokazali jsme se poprat se syntaktickymi chybami v podmince s MOD.
/*

CREATE OR REPLACE TRIGGER Don_Datum_Narozeni BEFORE INSERT OR UPDATE OF datum_narozeni ON don FOR EACH ROW
BEGIN

  IF((SUBSTR(:NEW.datum_narozeni, 4, 2) IN ('01', '03', '05', '07', '08', '10', '12'))) THEN      -- if months that have 31 days
    IF NOT ((SUBSTR(:NEW.datum_narozeni, 1, 2) IN ('01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31'))) THEN
      RAISE_APPLICATION_ERROR(-20000, 'v zadanom mesiaci moze byt najviac 31 dni');
    END IF;

  ELSIF ((SUBSTR(:NEW.datum_narozeni, 4, 2) IN ('04', '06', '09', '11'))) THEN             -- if months that have 30 days
    IF NOT ((SUBSTR(:NEW.datum_narozeni, 1, 2) IN ('01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31'))) THEN
      RAISE_APPLICATION_ERROR(-20000, 'v zadanom mesiac imoze byt najviac 30 dni');
    END IF;

  ELSIF ((SUBSTR(:NEW.datum_narozeni, 4, 2) IN ('02'))) THEN                            -- if february (29 always passes)
    IF ((SUBSTR(:NEW.datum_narozeni, 1, 2) IN ('01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29'))) THEN

      IF ((SUBSTR(:NEW.datum_narozeni, 1, 2) IN ('29'))) THEN
        IF NOT (MOD(CAST(SUBSTR(:NEW.datum_narozeni, 7, 4) AS int), 4)) THEN
            -- neni prechodny rok
          RAISE_APPLICATION_ERROR(-20001, 'Zadali ste 29. Februar ked neni Priestupny rok (neni delitelny 4)');
        ELSE
          IF (MOD(CAST(SUBSTR(:NEW.datum_narozeni, 7, 4) AS int), 100)) THEN
              -- neni prechodny rok
            RAISE_APPLICATION_ERROR(-20001, 'Zadali ste 29. Februar ked neni Priestupny rok (je delitelny 100)');
          ELSE
            IF NOT (MOD(CAST(SUBSTR(:NEW.datum_narozeni, 7, 4) AS int), 400)) THEN
                -- neni prechodny rok
              RAISE_APPLICATION_ERROR(-20001, 'Zadali ste 29. Februar ked neni Priestupny rok (neni delitelny 400)');
            END IF;
          END IF;
        END IF;
      END IF;
    ELSE
      RAISE_APPLICATION_ERROR(-20000, 'v zadanom mesiaci moze byt najviac 29 dni');
    END IF;
  ELSE 
    RAISE_APPLICATION_ERROR(-20002, 'mesiac neni v rozmedzi 1-12');
  END IF;

  IF (CAST(SUBSTR(:NEW.datum_narozeni, 7, 4) AS int) > 2001) THEN
    RAISE_APPLICATION_ERROR(-20003, 'clen mafie musi dovrsit dospelost');
  ELSE
    IF (CAST(SUBSTR(:NEW.datum_narozeni, 7, 4) AS int) < 1900) THEN
      RAISE_APPLICATION_ERROR(-20003, 'tak stary clovek uz nezije');
    END IF;
  END IF;
END;

*/




    -- vkladani konkretnich dat do tabulek
  -- Donove
INSERT INTO don(rodina, jmeno, datum_narozeni, velikost_bot)
  VALUES('Marquina', 'Sergio', '29-04-1943', 43);
INSERT INTO don(rodina, jmeno, datum_narozeni, velikost_bot)
  VALUES('Murillo', 'Raquel', '18-09-1987', 38);
INSERT INTO don(rodina, jmeno, datum_narozeni, velikost_bot)
  VALUES('Cano', 'Matias', '03-10-1974', 44);
INSERT INTO don(rodina, jmeno, datum_narozeni, velikost_bot)
  VALUES('Berrote', 'Martin', '01-10-1980', 42);
INSERT INTO don(rodina, jmeno, datum_narozeni, velikost_bot)
  VALUES('Dragic', 'Mirko', '04-09-1985', 44);


  -- Aliance
INSERT INTO aliance(doba_trvani, datum_vytvoreni)
  VALUES('157 dni', TO_DATE('4/8/2006', 'DD/MM/YYYY'));
INSERT INTO aliance(doba_trvani, datum_vytvoreni)
  VALUES('297 dni', TO_DATE('4/8/2007', 'DD/MM/YYYY'));
INSERT INTO aliance(doba_trvani, datum_vytvoreni)
  VALUES('1124 dni', TO_DATE('4/8/2002', 'DD/MM/YYYY'));

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




    -- demonstrace triggeru
  -- Předvedení triggeru (1):
SELECT id, adresa, rajon
FROM uzemi
ORDER BY id;


  -- Předvedení triggeru (2):
INSERT INTO don(rodina, jmeno, datum_narozeni, velikost_bot)
  VALUES('Thomas', 'Shelby', '29-02-1899', 43);  -- nespravne datum

SELECT rodina, jmeno, datum_narozeni  -- vsetci by mali mat spravne datum ([1-31]-[1-12]-[1901 - 2001]).
FROM don;




   -- procedury
  -- (1) Kolika setkani se don zucastnil.
CREATE OR REPLACE PROCEDURE pocet_setkani_dona(param_rodina IN VARCHAR)
AS
  cursor ucastni_se_setkani_radek IS SELECT * FROM ucastni_se_setkani;
  var_pocet_setkani NUMBER;
  var_ucastni_se_setkani ucastni_se_setkani_radek%ROWTYPE;
BEGIN

  var_pocet_setkani := 0;

  open ucastni_se_setkani_radek;
    loop
      fetch ucastni_se_setkani_radek into var_ucastni_se_setkani;
        exit when ucastni_se_setkani_radek%NOTFOUND;
      IF (var_ucastni_se_setkani.don = param_rodina) THEN
        var_pocet_setkani := var_pocet_setkani + 1;
      END IF;
    end loop;
  close ucastni_se_setkani_radek;

  IF var_pocet_setkani > 0 THEN
    dbms_output.put_line('Don ' || param_rodina || ' se ucastil: ' || var_pocet_setkani || ' setkani');
  ELSIF var_pocet_setkani = 0 THEN
    dbms_output.put_line('Don ' || param_rodina || ' se neucastil zadneho setkani');
  ELSE
    Raise_Application_Error (-20100, 'Error!');
  END IF;
END;
/


 -- (2) Kolikrat byl konkretni prostredek pouzit k trestne cinnosti.
CREATE OR REPLACE PROCEDURE opotrebovanost_prostredku(param_prostredek IN INT)
IS
  cursor provadi_cinnost_radek is select * from provadi_cinnost;
  var_pocet_pouziti NUMBER;
  var_provadi_cinnost provadi_cinnost_radek%ROWTYPE;
BEGIN

  var_pocet_pouziti := 0;

  open provadi_cinnost_radek;
    loop
      fetch provadi_cinnost_radek into var_provadi_cinnost;
        exit when provadi_cinnost_radek%NOTFOUND;
      IF (var_provadi_cinnost.prostredek = param_prostredek) THEN
        var_pocet_pouziti := var_pocet_pouziti + 1;
      END IF;
    end loop;
  close provadi_cinnost_radek;

  IF var_pocet_pouziti > 0 THEN
    dbms_output.put_line('Prostredek ' || param_prostredek || ' byl pouzit: ' || var_pocet_pouziti || ' krat');
  ELSIF var_pocet_pouziti = 0 THEN
    dbms_output.put_line('Prostredek ' || param_prostredek || ' se jeste nebyl pouzit');
  ELSE
    Raise_Application_Error (-20101, 'Error!');
  END IF;
END;
/


    -- příklad spuštění procedur
  BEGIN pocet_setkani_dona('Marquina'); END;
  /
  BEGIN opotrebovanost_prostredku(2); END;
  /




    -- EXPLAIN PLAN, index
  -- jestli nejaka rodina nema zadnou mladou krev, tak brzo zanikne (stari nezvladnou vykonavat spinavou robotu).
EXPLAIN PLAN FOR
SELECT
    d.rodina AS rodina,
    COUNT(rc.vek) AS pocet_novacku
FROM radovy_clen rc
JOIN don d ON d.rodina = rc.rodina
GROUP BY d.rodina
HAVING COUNT(rc.vek) < 30
ORDER BY pocet_novacku;
  -- výpis
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

  -- Index pro email uživatele (seskupení podle emailu a rychlejší vyhledávání).
DROP INDEX radovy_clen_vek;
CREATE INDEX radovy_clen_vek ON radovy_clen(rodina, vek);

  -- druhý pokus
EXPLAIN PLAN FOR
SELECT
    d.rodina AS rodina,
    COUNT(rc.vek) AS pocet_novacku
FROM radovy_clen rc
JOIN don d ON d.rodina = rc.rodina
GROUP BY d.rodina
HAVING COUNT(rc.vek) < 30
ORDER BY pocet_novacku;
  -- výpis
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);




    -- Materializovaný pohled
  --  pocet aliancii jdnotlivych rodin.
DROP MATERIALIZED VIEW pocet_aliancii;

CREATE MATERIALIZED VIEW pocet_aliancii AS
SELECT p.rodina, COUNT(p.aliance) AS "pocet_pratel"
FROM prislusnost_k_alianci p
GROUP BY p.rodina;

  -- Výpis materializovaného pohledu.
SELECT * FROM pocet_aliancii;

  -- Aktualizace dat, které jsou v materializovaném pohledu.
UPDATE prislusnost_k_alianci SET rodina = 'Cano' WHERE rodina = 'Marquina';

-- Data se v materializovaném pohledu neaktualizují.
SELECT * FROM pocet_aliancii;




--definici přístupových práv k databázovým objektům pro druhého člena týmu,

GRANT ALL ON don TO xpospi0f;
GRANT ALL ON aliance TO xpospi0f;
GRANT ALL ON uzemi TO xpospi0f;

GRANT ALL ON prislusnost_k_alianci TO xpospi0f;
GRANT ALL ON setkani_donu TO xpospi0f;
GRANT ALL ON ucastni_se_setkani TO xpospi0f;

GRANT ALL ON radovy_clen TO xpospi0f;
GRANT ALL ON cinnost TO xpospi0f;
GRANT ALL ON prostredky TO xpospi0f;
GRANT ALL ON vrazda TO xpospi0f;
GRANT ALL ON provadi_cinnost TO xpospi0f;

GRANT EXECUTE ON pocet_setkani_dona TO xpospi0f;
GRANT EXECUTE ON opotrebovanost_prostredku TO xpospi0f;

GRANT ALL ON pocet_aliancii TO xpospi0f;