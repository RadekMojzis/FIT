-- Skript s priklady na varianty spojeni, predpoklada 
-- vytvorenou a naplnenou databazi
--
SELECT * FROM Klient;
SELECT * FROM Ucet;
-- spojeni pomoci podminky ve WHERE
SELECT K.jmeno, U.c_uctu
FROM Klient K, Ucet U
WHERE K.r_cislo=U.r_cislo;

-- spojeni pomoci JOIN ON
SELECT K.jmeno, U.c_uctu
FROM Klient K JOIN Ucet U ON K.r_cislo=U.r_cislo;

SELECT *
FROM Klient K JOIN Ucet U ON K.r_cislo=U.r_cislo;

-- spojeni pomoci JOIN USING
SELECT jmeno, c_uctu
FROM Klient JOIN Ucet USING (r_cislo);

-- spojeni pomoci JOIN USING - sloupce v USING nesmí být kvalifikované
SELECT K.jmeno, U.c_uctu
FROM Klient K JOIN Ucet U USING (r_cislo);

SELECT K.jmeno, U.c_uctu
FROM Klient K JOIN Ucet U USING (K.r_cislo);

SELECT K.jmeno, U.c_uctu, K.r_cislo
FROM Klient K JOIN Ucet U USING (r_cislo);

SELECT *
FROM Klient JOIN Ucet USING (r_cislo);

-- spojeni pomoci JOIN USING - jen nìkteréze stejných sloupcù použity
SELECT *
FROM Klient K1 JOIN Klient K2 USING (ulice)
WHERE K1.r_cislo>K2.r_cislo;

-- spojeni pomoci NATURAL JOIN - spoleèné sloupce nekvalifikované
SELECT jmeno, c_uctu
FROM Klient NATURAL JOIN Ucet;

SELECT *
FROM Klient NATURAL JOIN Ucet;

-- vnejsi spojeni
SELECT jmeno
FROM Klient;

SELECT jmeno, c_uctu
FROM Klient NATURAL JOIN Ucet;

SELECT jmeno, c_uctu
FROM Klient NATURAL LEFT JOIN Ucet;

-- Kteøí klienti nemají úèet
SELECT jmeno
FROM Klient NATURAL LEFT JOIN Ucet
WHERE c_uctu IS NULL;

-- NULL ve sloupci pro spojení - funkce COALESCE, 
-- tj. zùstane tam NULL
CREATE TABLE T1 (
a INTEGER,
b INTEGER,
c INTEGER);

CREATE TABLE T2 (
a INTEGER,
b INTEGER,
d INTEGER);

INSERT INTO T1
VALUES (1, 1, 1);
INSERT INTO T1
VALUES (1, 2, 2);
INSERT INTO T1
VALUES (1,NULL, 3);

INSERT INTO T2
VALUES (1, NULL, 1);
INSERT INTO T2
VALUES (1, 2, 2);

SELECT * FROM T1;
SELECT * FROM T2;

SELECT *
FROM T1 NATURAL JOIN T2;
SELECT *
FROM T1 JOIN T2 USING(a,b);

SELECT *
FROM T1 NATURAL LEFT JOIN T2;
SELECT *
FROM T1 LEFT JOIN T2 USING(a,b);

SELECT *
FROM T1 NATURAL RIGHT JOIN T2;
SELECT *
FROM T1 RIGHT JOIN T2 USING(a,b);

DROP TABLE T1;
DROP TABLE T2;