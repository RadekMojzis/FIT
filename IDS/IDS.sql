



CREATE TABLE Zamestnanec (
cislo_zamestnance INTEGER,
jmeno          VARCHAR(20),
prijmeni       VARCHAR(20),
); 

CREATE TABLE Lekar (
email          VARCHAR(40),
specializace   VARCHAR(20),
); 

CREATE TABLE Sestra (
uvazek         VARCHAR(20), -- Ne... fakt nemam ponetí v jakých jednotkách se udává úvazek :D
Platova_trida  VARCHAR(20),
);

CREATE TABLE Oddeleni (
cislo_oddeleni INTEGER,
nazev          VARCHAR(20),
budova         VARCHAR(20),
); 

CREATE TABLE vysetreni (
cislo_vysetreni INTEGER,
datum          DATE,
vysledek       VARCHAR(20),
); 

CREATE TABLE Lek (
cislo_leku     INTEGER,
nazev          VARCHAR(20),
ucinna_latka   VARCHAR(20),
sila           VARCHAR(20)
); 

CREATE TABLE Pacient (
cislo_zamestnance INTEGER,
jmeno    VARCHAR(20) NOT NULL,
prijmeni VARCHAR(20) NOT NULL,
); 



ALTER TABLE Klient ADD CONSTRAINT PK_klient PRIMARY KEY (r_cislo);