-- Skript pro vytvoreni databaze pouzite v prikladech 
-- DML SQL, zejmena SELECT
-- 
--
DROP TABLE Transakce CASCADE CONSTRAINTS;
DROP TABLE Ucet CASCADE CONSTRAINTS;
DROP TABLE Pobocka CASCADE CONSTRAINTS;
DROP TABLE Klient CASCADE CONSTRAINTS;

DROP TABLE Pujcka CASCADE CONSTRAINTS;


CREATE TABLE Klient (
r_cislo  CHAR(11),
jmeno VARCHAR(20) NOT NULL,
ulice VARCHAR(20),
mesto VARCHAR(20)
); 

CREATE TABLE Pobocka (
nazev VARCHAR(20),
jmeni INTEGER
); 

CREATE TABLE UCET (
c_uctu  INTEGER,
stav DECIMAL(10,2),
r_cislo CHAR(11) NOT NULL,
pobocka VARCHAR(20)
); 

CREATE TABLE Transakce (
c_uctu  INTEGER,
c_transakce INTEGER,
datum DATE,
castka DECIMAL(7,2)
); 

ALTER TABLE Klient ADD CONSTRAINT PK_klient PRIMARY KEY (r_cislo);
ALTER TABLE Pobocka ADD CONSTRAINT PK_pobocka PRIMARY KEY (nazev);
ALTER TABLE Ucet ADD CONSTRAINT PK_ucet PRIMARY KEY (c_uctu);
ALTER TABLE Ucet ADD CONSTRAINT FK_ucet_rcislo FOREIGN KEY (r_cislo) REFERENCES Klient ON DELETE CASCADE;
ALTER TABLE Ucet ADD CONSTRAINT FK_ucet_pobocka FOREIGN KEY (pobocka) REFERENCES Pobocka;
ALTER TABLE Transakce ADD CONSTRAINT PK_transakce PRIMARY KEY (c_uctu,c_transakce);
ALTER TABLE Transakce ADD CONSTRAINT FK_transakce_cuctu FOREIGN KEY (c_uctu) REFERENCES Ucet ON DELETE CASCADE;


INSERT INTO Klient 
VALUES('440726/0672','Jan Nov�k','Cejl 8','Brno');
INSERT INTO Klient
VALUES('530610/4532','Petr Vesel�','Podzimn� 28','Brno');
INSERT INTO Klient
VALUES('601001/2218','Ivan Zeman ','Cejl 8','Brno');
INSERT INTO Klient
VALUES('510230/048','Pavel Tomek','Tomkova 34','Brno');
INSERT INTO Klient
VALUES('580807/9638','Josef M�dr','Svatoplukova 15','Brno');
INSERT INTO Klient
VALUES('625622/6249','Jana Mal�','Brn�nsk� 56','Vy�kov');

INSERT INTO Pobocka
VALUES('J�nsk�',10000000);
INSERT INTO Pobocka
VALUES('Palack�ho',5000000);
SELECT * FROM Pobocka;

INSERT INTO Ucet
VALUES(4320286,52000,'440726/0672','J�nsk�');
INSERT INTO Ucet
VALUES(2348531,10000,'530610/4532','J�nsk�');
INSERT INTO Ucet
VALUES(1182648,10853,'530610/4532','Palack�ho');
INSERT INTO Ucet
VALUES(2075752,26350,'440726/0672','Palack�ho');
SELECT * FROM Ucet;

INSERT INTO Transakce
VALUES(4320286,1,TO_DATE('10.10.1998', 'dd.mm.yyyy'),3000);
INSERT INTO Transakce
VALUES(4320286,2,TO_DATE('12.10.1998', 'dd.mm.yyyy'),-5000);
INSERT INTO Transakce
VALUES(2075752,1,TO_DATE('14.10.1998', 'dd.mm.yyyy'),-2000);
INSERT INTO Transakce
VALUES(2075752,2,TO_DATE('14.10.1998', 'dd.mm.yyyy'),10000);

COMMIT;

SELECT * FROM Klient;
SELECT * FROM Pobocka;
SELECT * FROM Ucet;
SELECT * FROM Transakce;

SELECT r_cislo,jmeno
FROM Klient NATURAL JOIN Ucet NATURAL JOIN Transakce;

SELECT *
FROM Klient NATURAL JOIN Ucet NATURAL JOIN Transakce;

SELECT r_cislo,jmeno
FROM (Klient NATURAL JOIN Ucet) JOIN Transakce ON Transakce.c_uctu=Ucet.c_uctu;

SELECT r_cislo,jmeno
FROM (Klient NATURAL JOIN Ucet) JOIN Transakce USING (c_uctu);

SELECT r_cislo,jmeno, c_uctu
FROM (Klient NATURAL JOIN Ucet) NATURAL JOIN Transakce;

SELECT r_cislo,jmeno, c_uctu, pobocka, c_transakce
FROM Klient NATURAL JOIN Ucet NATURAL JOIN Transakce;

SELECT *
FROM Klient NATURAL JOIN Ucet NATURAL JOIN Transakce;

SELECT Klient.r_cislo,jmeno
FROM (Klient JOIN Ucet ON Klient.r_cislo=Ucet.r_cislo) JOIN Transakce ON Ucet.c_uctu=Transakce.c_uctu;

SELECT *
FROM (Klient NATURAL JOIN Ucet) NATURAL JOIN Transakce;

SELECT r_cislo, jmeno
FROM (Klient NATURAL JOIN Ucet); 

SELECT *
FROM (Klient NATURAL JOIN Ucet); 

SELECT c_uctu
FROM Ucet NATURAL JOIN Transakce;
