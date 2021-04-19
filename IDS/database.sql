-- IDS projekt 2020/21
-- xoleks00, xkromp00

----- ULOHA 2 -----

DROP TABLE Vybavuje;
DROP TABLE Poziadavky;
DROP TABLE Zakazky;
DROP TABLE FyzickeOsoby;
DROP TABLE PravnickeOsoby;
DROP TABLE Faktury;
DROP TABLE Zamestnanci;
DROP TABLE Oddelenia;

-- miestnost je stale oznacena trojcifernym císlom
CREATE TABLE Oddelenia (
    nazov VARCHAR(50) PRIMARY KEY,
    popis VARCHAR(100),
    miestnost INT NOT NULL CHECK (miestnost >= 100 AND miestnost <= 999)
);
CREATE TABLE Zamestnanci (
    id INT GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    mzda INT NOT NULL CHECK(mzda >= 0),
    titul VARCHAR(20),
    specializacia VARCHAR(50),
    meno VARCHAR(50) NOT NULL,
    oddelenie VARCHAR(50) REFERENCES Oddelenia NOT NULL,
    vedeneOddelenie VARCHAR(50) NULL
);
CREATE TABLE Faktury (
    cislo INT GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    suma INT CHECK(suma >= 0) NOT NULL,
    datumZadania DATE NOT NULL,
    terminDodania DATE NOT NULL,
    vystavujuciZamestnanec INT REFERENCES Zamestnanci NOT NULL
);
-- klient sa specializuje na pravnicku osobu alebo na fyzicku osobu
-- prislusnost je disjunktna a uplnost je totalna
-- ico je osemciferne číslo
CREATE TABLE PravnickeOsoby (
    ico INT PRIMARY KEY CHECK (ico >= 10000000 AND ico <= 99999999),
    adresa VARCHAR(100) NOT NULL,
    dic CHAR(12) UNIQUE NOT NULL,
    telefonneCislo VARCHAR(20),
    obchodneMeno VARCHAR(50) NOT NULL,
    menoStatutarnehoOrganu VARCHAR(50) NOT NULL,
    cisloVObchodnomRegistri VARCHAR(50) UNIQUE,
    pravnaForma VARCHAR(100),
    bonita INT CHECK(bonita >= 0)
);
CREATE TABLE FyzickeOsoby (
    ico INT PRIMARY KEY CHECK (ico >= 10000000 AND ico <= 99999999),
    adresa VARCHAR(100) NOT NULL,
    dic CHAR(12) UNIQUE NOT NULL,
    telefonneCislo VARCHAR(20),
    meno VARCHAR(100) NOT NULL,
    cisloZivnostenskehoRegistra VARCHAR(20) UNIQUE
);
CREATE TABLE Zakazky (
    cisloZakazky INT GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    stav VARCHAR(20) CHECK( stav IN ('objednana', 'zaplatena', 'prebieha', 'dokoncena', 'stornovana') ) NOT NULL,
    datumZadania DATE NOT NULL,
    celkovaCena INT CHECK(celkovaCena >= 0) NOT NULL,
    terminDodania DATE NOT NULL,
    icoPravnickej INT NULL CHECK (icoPravnickej >= 10000000 AND icoPravnickej <= 99999999),
    icoFyzickej INT NULL CHECK (icoFyzickej >= 10000000 AND icoFyzickej <= 99999999),
    zodpovednyZamestnanec INT REFERENCES Zamestnanci,
    CONSTRAINT fk1 FOREIGN KEY (icoPravnickej) REFERENCES PravnickeOsoby,
    CONSTRAINT fk2 FOREIGN KEY (icoFyzickej) REFERENCES FyzickeOsoby
);
CREATE TABLE Poziadavky (
    cisloZakazky INT,
    poradoveCislo INT,
    stav VARCHAR(20) CHECK( stav IN ('objednana', 'zaplatena', 'prebieha', 'dokoncena', 'stornovana') ),
    typ VARCHAR(20) CHECK( typ IN ('internetova kampan', 'televizna kampan', 'letakova kampan', 'billboardova kampan') ),
    cena INT CHECK(cena >= 0),
    dobaPlatnosti DATE,
    pocetBrigadnikov INT CHECK(pocetBrigadnikov >= 0),
    cisloFaktury INT REFERENCES Faktury,
    FOREIGN KEY (cisloZakazky) REFERENCES Zakazky,
    CONSTRAINT pk PRIMARY KEY (cisloZakazky, poradoveCislo)
);

CREATE TABLE Vybavuje (
    oddelenie VARCHAR(50) REFERENCES Oddelenia,
    cisloZakazky INT,
    poradoveCisloPoziadavky INT,
    CONSTRAINT fk3 FOREIGN KEY (cisloZakazky, poradoveCisloPoziadavky) REFERENCES Poziadavky,
    CONSTRAINT pk2 PRIMARY KEY (oddelenie, cisloZakazky, poradoveCisloPoziadavky)
);

-- ukazkove data
INSERT INTO Oddelenia VALUES ('Ekonomicke', 'Vyplaty a dane', 512);
INSERT INTO Oddelenia VALUES ('Personalne', 'Najimanie ludi', 515);
INSERT INTO Oddelenia VALUES ('Marketingove', 'Komunikacia so zakaznikmi a s reklamnymi spolocnostami', 103);
INSERT INTO Oddelenia VALUES ('Analyticke', 'Zber a spracovanie dat', 430);

INSERT INTO Zamestnanci VALUES (DEFAULT, 3000, 'Ing.', 'Danovy system SR', 'Monika Krompascikova', 'Ekonomicke', 'Ekonomicke');
INSERT INTO Zamestnanci VALUES (DEFAULT, 1800, NULL, 'IT', 'Emma Krompascikova', 'Ekonomicke', NULL);
INSERT INTO Zamestnanci VALUES (DEFAULT, 3200, 'Mgr.', 'Psychologia osobnosti', 'Lucia Kruliacova', 'Personalne', 'Personalne');
INSERT INTO Zamestnanci VALUES (DEFAULT, 2800, NULL, 'IT', 'Michal Findra', 'Personalne', NULL);
INSERT INTO Zamestnanci VALUES (DEFAULT, 3000, 'Ing.', 'IT', 'Samuel Oleksak', 'Marketingove', 'Marketingove');
INSERT INTO Zamestnanci VALUES (DEFAULT, 1200, NULL, 'Multilingvalna komunikacia', 'Ella Krompascikova', 'Marketingove', NULL);
INSERT INTO Zamestnanci VALUES (DEFAULT, 2500, 'Ing.', 'Štatistika', 'Jan Novotny', 'Analyticke', NULL);
INSERT INTO Zamestnanci VALUES (DEFAULT, 3500, 'Ing.', 'Štatistika', 'Igor Vesely', 'Analyticke', 'Analyticke');

INSERT INTO PravnickeOsoby VALUES (31735657, 'Nizna brana 2, Kezmarok, SR', 'SK4540607291', '+421911123791', 'Kavomaty', 'Ing. Peter Grondžák', '3378/P', 's.r.o.', 450000);
INSERT INTO PravnickeOsoby VALUES (44327960, 'Biela 1, Kosice, SR', 'SK4832700164', '+421911486555', 'VODEX', 'Ing. Viktor Pastirik', '22361/V', 's.r.o.', 100000);
INSERT INTO PravnickeOsoby VALUES (31730574, 'Hranicna 17, Poprad, SR', 'SK4811100164', '+42184564564', 'Veolia Energia Poprad', 'Ing. Peter Dobrý', '292/P', 'a.s.', 2300000);

INSERT INTO FyzickeOsoby VALUES (45897312, 'Jarna 22, Svidnik, SR', 'SK1230005678', '+421987453102', 'Juraj Mokry', '564-5654');
INSERT INTO FyzickeOsoby VALUES (40128164, 'Slavkovska 19, Kezmarok, SR', 'SK1248592678', '+421948753102', 'Marta Benckova', '366-1254');

INSERT INTO Zakazky VALUES (DEFAULT, 'objednana', '4-4-2021', 4530, '4-5-2021', 31735657, NULL, 5);
INSERT INTO Zakazky VALUES (DEFAULT, 'prebieha', '3-4-2021', 350, '3-5-2021', 31735657, NULL, 6);
INSERT INTO Zakazky VALUES (DEFAULT, 'zaplatena', '4-4-2021', 1520, '4-5-2021', NULL, 40128164, 5);
INSERT INTO Zakazky VALUES (DEFAULT, 'prebieha', '3-4-2021', 844, '3-5-2021', NULL, 40128164, 6);
INSERT INTO Zakazky VALUES (DEFAULT, 'prebieha', '3-4-2021', 1020, '3-5-2021', 44327960, NULL, 6);

INSERT INTO Faktury VALUES (DEFAULT, 4530, '4-4-2021', '4-5-2021', 5);
INSERT INTO Faktury VALUES (DEFAULT, 350, '3-4-2021', '3-5-2021', 6);
INSERT INTO Faktury VALUES (DEFAULT, 1520, '4-4-2021', '4-5-2021', 5);
INSERT INTO Faktury VALUES (DEFAULT, 500, '3-4-2021', '3-5-2021', 6);
INSERT INTO Faktury VALUES (DEFAULT, 344, '3-4-2021', '3-5-2021', 5);

INSERT INTO Poziadavky VALUES (1, 1, 'objednana', 'letakova kampan', 2530, '1-5-2021', 3, 1);
INSERT INTO Poziadavky VALUES (1, 2, 'objednana', 'internetova kampan', 2000, '15-4-2021', 0, 1);
INSERT INTO Poziadavky VALUES (2, 1, 'prebieha', 'letakova kampan', 2530, '1-5-2021', 3, 2);
INSERT INTO Poziadavky VALUES (3, 1, 'prebieha', 'televizna kampan', 2530, '1-5-2021', 3, 3);
INSERT INTO Poziadavky VALUES (4, 1, 'dokoncena', 'letakova kampan', 2530, '5-5-2021', 2, 4);
INSERT INTO Poziadavky VALUES (4, 2, 'dokoncena', 'billboardova kampan', 2530, '10-5-2021', 1, 5);

INSERT INTO Vybavuje VALUES ('Marketingove', 1, 1);
INSERT INTO Vybavuje VALUES ('Analyticke', 1, 2);
INSERT INTO Vybavuje VALUES ('Marketingove', 1, 2);
INSERT INTO Vybavuje VALUES ('Marketingove', 2, 1);
INSERT INTO Vybavuje VALUES ('Marketingove', 3, 1);
INSERT INTO Vybavuje VALUES ('Analyticke', 3, 1);
INSERT INTO Vybavuje VALUES ('Marketingove', 4, 1);
INSERT INTO Vybavuje VALUES ('Marketingove', 4, 2);

/*
SELECT * FROM Vybavuje;
SELECT * FROM Poziadavky;
SELECT * FROM Zakazky;
SELECT * FROM FyzickeOsoby;
SELECT * FROM PravnickeOsoby;
SELECT * FROM Faktury;
SELECT * FROM Zamestnanci;
SELECT * FROM Oddelenia;
*/

----- ULOHA 3 -----

-- JOIN dvoch tabuliek --
-- Zamestnanec ma problem s pocitacom a potrebuje najst v akej miestosti sa nachadza kolega s IT specializaciou
-- Vyber take miestnosti, kde sa nachadza zamestnanec s IT specializaciou
-- Na zistenie miestnosti musim spojit tabulky Oddelenia a Zamestnanci
SELECT DISTINCT
    o.miestnost
FROM
         oddelenia o
    JOIN zamestnanci z ON z.oddelenie = o.nazov
WHERE
    z.specializacia = 'IT';

-- JOIN dvoch tabuliek --
-- Hladam cislo zakaziek, ktore maju vsetky poziadavky dokoncene, aby som mohol celu zakazku prehlasit za dokoncenu
-- Spajam tabulky Poziadavky a Zakazky
SELECT DISTINCT
    p.cislozakazky
FROM
    poziadavky  p
    LEFT JOIN zakazky     z ON p.cislozakazky = z.cislozakazky
WHERE
    p.stav = 'dokoncena';

-- JOIN troch tabuliek --
-- Chcem zistit kolko brigadnikov bude ktore oddelenie potrebovat na splnenie poziavaviek
-- Kedze poziadavky a oddelenie su spojene vztahom vybavuje s kardinalitou n ku n, tak tento vztah musi byt vyjadreny
-- samostatnou tabulkou - Vybavuje, ktoru spajam s tabulkami Oddelenia a Poziadavky
SELECT
    v.oddelenie,
    SUM(p.pocetbrigadnikov)
FROM
         vybavuje v
    JOIN poziadavky  p ON p.cislozakazky = v.cislozakazky
                         AND p.poradovecislo = v.poradovecislopoziadavky
    JOIN oddelenia   o ON o.nazov = v.oddelenie
GROUP BY
    v.oddelenie;

-- vyuzitie agregacnej funkcie spolu s GROUP BY --
-- Klient, ktoreho sucet cien zakazok prekrocil 1000€ dostane postou vernostnu zlavu
-- Agregacnou funkciou SUM scitam sucet cien zakazok vsetkych klientov a vyberame adresu tych, ktori maju sucet > 1000
SELECT
    SUM(z.celkovacena) sucet,
    k.adresa
FROM
         zakazky z
    JOIN pravnickeosoby k ON z.icopravnickej = k.ico
GROUP BY
    k.adresa
HAVING
    SUM(z.celkovacena) > 1000
UNION
SELECT
    SUM(z.celkovacena) sucet,
    k.adresa
FROM
         zakazky z
    JOIN fyzickeosoby k ON z.icofyzickej = k.ico
GROUP BY
    k.adresa
HAVING
    SUM(z.celkovacena) > 1000;

-- vyuzitie agregacnej funkcie spolu s GROUP BY --
-- Zaujemca o zamestnanie je zvedavy aky je priemerny plat v jednotlivych oddeleniach nasej firmy
-- Vyuzijem agregacnu funkciu AVG na stlpec mzda kazdeho zamestnanca v ramci jednotlivych oddeleni
SELECT
    o.nazov,
    AVG(mzda)
FROM
         zamestnanci z
    JOIN oddelenia o ON z.oddelenie = o.nazov
GROUP BY
    o.nazov;

-- vyuzitie klauzule EXISTS --
-- Chceme vybrat mena zamestnancov, ktori zodpovedaju aspon za jednu zakazku, aby sme im vyplatili premiu
-- Vyberam mena tych zamestnancov, kde existuje aspon jeden riadok v tabulke zakazky, ktory ma ich id ako zodpovedneho zamestnanca
SELECT
    zamestnanci.meno
FROM
    zamestnanci
WHERE
    EXISTS (
        SELECT
            zakazky.zodpovednyzamestnanec
        FROM
            zakazky
        WHERE
            zamestnanci.id = zakazky.zodpovednyzamestnanec
    );

-- vyuzitie klauzuly IN (SELECT ...)
-- Vyberam take zakazky, ktorych poziadavky vybavuje iba jedno oddelenie, tieto zakazky su jednoduchsie na vybavenie a su vybavene prioritne
-- Vyuzivam vnorenu klauzulu select, ktora spaja tri tabulky a agreguje pocet oddeleni na zakazku
SELECT
    *
FROM
    zakazky
WHERE
    cislozakazky IN (
        SELECT
            v.cislozakazky
        FROM
                 vybavuje v
            JOIN poziadavky  p ON p.cislozakazky = v.cislozakazky
                                 AND p.poradovecislo = v.poradovecislopoziadavky
            JOIN oddelenia   o ON o.nazov = v.oddelenie
        GROUP BY
            v.cislozakazky
        HAVING
            COUNT(DISTINCT v.oddelenie) = 1
    );
    