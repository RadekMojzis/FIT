-- Skript s priklady na prikaz SELECT, predpoklada 
-- vytvorenou a naplnenou databazi
--
SELECT * FROM Klient;
SELECT * FROM Ucet;
SELECT * FROM Pobocka;
SELECT * FROM Transakce;

-- Kte�� klienti maj� ��et u spo�itelny?
SELECT * 
FROM Klient;

-- Z kter�ch m�st jsou klienti spo�itelny?
SELECT DISTINCT mesto
FROM Klient;

SELECT mesto
FROM Klient;

-- Kter� ��ty jsou u pobo�ky J�nsk�?
SELECT *
FROM Ucet
WHERE pobocka = 'J�nsk�';

-- Kolik �in� jm�n� pobo�ek v USD pri kurzu 25 K�/$?
SELECT nazev, jmeni/25 
FROM Pobocka;

SELECT nazev, jmeni/25 AS jmeni_v_$ 
FROM Pobocka;

SELECT nazev, ' Jm�n� v USD =' AS jmeni_v_$, jmeni/25 
FROM Pobocka;

-- Uspo��d�n� pomoc� ORDER BY
SELECT nazev, jmeni/25 AS jmeni_v_$ 
FROM Pobocka 
ORDER BY nazev;

SELECT nazev, jmeni/25 AS jmeni_v_$ 
FROM Pobocka 
ORDER BY nazev DESC;

SELECT nazev, jmeni/25 AS jmeni_v_$ 
FROM Pobocka 
ORDER BY jmeni_v_$;

SELECT nazev, jmeni/25 AS jmeni_v_$ 
FROM Pobocka 
ORDER BY 2 DESC;

-- Kte�� klienti maj� ��et v pobo�ce J�nsk�
SELECT r_cislo,jmeno FROM Klient;
SELECT * FROM Ucet;

SELECT DISTINCT K.*
FROM Klient K, Ucet U
WHERE K.r_cislo=U.r_cislo AND pobocka='J�nsk�';

-- Kte�� klienti prov�d�li transakce v pobo�ce J�nsk� 12.10.1998?
SELECT r_cislo,jmeno FROM Klient;
SELECT * FROM Ucet;
SELECT * FROM Transakce;

SELECT K.r_cislo, K.jmeno, T.c_uctu, castka
FROM Klient K, Ucet U, Transakce T
WHERE K.r_cislo=U.r_cislo AND U.c_uctu=T.c_uctu 
            AND pobocka='J�nsk�' AND datum=TO_DATE('1998-10-12','YYYY-MM-DD')
ORDER BY K.r_cislo;

-- Kte�� klienti prov�d�li transakce v pobo�ce J�nsk� 12.10.1998? JOIN
SELECT r_cislo,jmeno FROM Klient;
SELECT * FROM Ucet;
SELECT * FROM Transakce;

SELECT r_cislo, jmeno, c_uctu, castka
FROM Klient NATURAL JOIN Ucet NATURAL JOIN Transakce
WHERE pobocka='J�nsk�' AND datum=TO_DATE('1998-10-12','YYYY-MM-DD')
ORDER BY r_cislo;

-- Pozor na chybu n�sobn�ho pou�it� NATURAL JOIN ve verz�ch < 11.2
-- �e�en�:
SELECT r_cislo, jmeno, c_uctu, castka
FROM Klient JOIN Ucet USING (r_cislo) JOIN Transakce USING (c_uctu)
WHERE pobocka='J�nsk�' AND datum=TO_DATE('1998-10-12','YYYY-MM-DD')
ORDER BY r_cislo;

-- Bydl� n�kte�� klienti ve stejn�m dom�?
SELECT r_cislo, ulice, mesto FROM Klient;

SELECT K1.jmeno,K1.r_cislo, K2.jmeno, K2.r_cislo, K1.ulice, K1.mesto
FROM Klient K1, Klient K2
WHERE K1.mesto=K2.mesto AND K1.ulice=K2.ulice AND K1.r_cislo>K2.r_cislo;

SELECT K1.jmeno,K1.r_cislo, K2.jmeno, K2.r_cislo, K1.ulice, K1.mesto
FROM Klient K1, Klient K2
WHERE K1.mesto=K2.mesto AND K1.ulice=K2.ulice;

SELECT K1.jmeno,K1.r_cislo, K2.jmeno, K2.r_cislo, K1.ulice, K1.mesto
FROM Klient K1, Klient K2
WHERE K1.mesto=K2.mesto AND K1.ulice=K2.ulice AND K1.r_cislo<>K2.r_cislo;

SELECT K1.jmeno,K1.r_cislo,K2.jmeno,K2.r_cislo,ulice,mesto
FROM Klient K1 JOIN Klient K2 USING (mesto,ulice)
WHERE K1.r_cislo>K2.r_cislo;

SELECT K1.jmeno,K1.r_cislo,K2.jmeno,K2.r_cislo,K1.ulice,K1.mesto
FROM Klient K1 JOIN Klient K2 ON K1.mesto=K2.mesto AND K1.ulice=K2.ulice
AND K1.r_cislo>K2.r_cislo;

-- Kolik klient� m� spo�itelna?
SELECT r_cislo,jmeno from Klient;

SELECT COUNT(*) AS pocet
FROM Klient;


-- Jak� byl maxim�ln� vklad?
SELECT * FROM Transakce;

SELECT MAX(castka) AS max_vklad
FROM Transakce;

-- Neni GROUP BY, bere se jako jedna skupina, 
-- tj. nelze kombinovat skupinov�/agrega�n� a neskupinov�

SELECT MAX(castka) AS max_vklad, AVG (castka) AS prumer
FROM Transakce;

SELECT c_uctu, MAX(castka) AS max_vklad
FROM Transakce;

SELECT * FROM Ucet;

-- Jak� ��stka je na ��tech v jednotliv�ch pobo�k�ch
SELECT pobocka, SUM(stav) AS celkem_na_uctech
FROM Ucet
GROUP BY pobocka;

--Kolik maj� ��t� a na nich pen�z jednotliv� klienti?
SELECT * FROM Ucet;

SELECT jmeno, r_cislo, COUNT(*) AS pocet, SUM(stav) AS celkem
FROM Klient NATURAL JOIN Ucet
GROUP BY jmeno,r_cislo;

SELECT jmeno, r_cislo, COUNT(*) pocet,SUM(stav) celkem
FROM Klient NATURAL JOIN Ucet
GROUP BY r_cislo, jmeno;


--Pou�it� vn�j��ho spojen�
SELECT jmeno, r_cislo, COUNT(c_uctu) AS pocet, SUM(stav) AS celkem
FROM Klient NATURAL LEFT JOIN Ucet
GROUP BY jmeno,r_cislo
ORDER BY 4 DESC;

-- Zano�ov�n� agrega�n�ch funkc�
--------------------------------
SELECT * FROM UCET;

-- Kolik maj� jednotliv� klienti pr�m�rn� na ��tu?
SELECT r_cislo, AVG(stav) AS "pr�m�r na ��tu"
FROM Ucet
GROUP BY r_cislo;

-- Chceme-li i jm�no
SELECT r_cislo, jmeno, AVG(stav) "pr�m�r na ��tu"
FROM Ucet JOIN Klient USING(r_cislo)
GROUP BY r_cislo, jmeno;

-- nebo (pou�it� dotazu ve FROM)
SELECT r_cislo, jmeno, prumer "pr�m�r na ��tu"
FROM (SELECT r_cislo, AVG(stav) prumer
      FROM Ucet
      GROUP BY r_cislo) 
      JOIN Klient USING(r_cislo);

-- max. pr�m�r vyu�it�m zano�en� agrega�n�ch funkc�(Oracle)
SELECT MAX(AVG(stav)) "nejvy��� pr�m�r na ��tu"
FROM Ucet
GROUP BY r_cislo;

-- bez zano�en� poddotaz ve FROM
SELECT MAX(prumer)
FROM (SELECT (AVG(stav)) prumer
     FROM Ucet
     GROUP BY r_cislo)
;

-- tabulku poddotazu ve FROM lze pojmenovat
SELECT MAX(P.prumer)
FROM (SELECT (AVG(stav)) prumer
     FROM Ucet
     GROUP BY r_cislo) P
;

-- Takto nelze
SELECT r_cislo, MAX(AVG(stav)) "nejvy��� pr�m�r na ��tu"
FROM Ucet
GROUP BY r_cislo;

SELECT * FROM UCET;

-- Vyu�it� se�azen� ("Top K query")
-- Pomoc� ORDER BY
-- Dle standardu (a� 2008 FETCH FIRST n ROWS ONLY, 
-- 2003 pomoc� funkce ROW_NUMBER()OVER (ORDER BY key ASC)
-- MySQL, PostgerSQL LIMIT
-- Oracle pseudosloupec ROWNUM
SELECT *
FROM (SELECT AVG(stav) AS prumer
      FROM Ucet
      GROUP BY r_cislo
      ORDER BY prumer DESC)
WHERE ROWNUM = 1;

-- P�edchoz� ne�e�� p��pad, kdy je n�kolik s nejv�t�� hodnotou.
Obecn�ji lze �e�it takto:
SELECT R_cislo,AVG(stav) AS prumer
FROM Ucet
GROUP BY r_cislo
HAVING AVG(stav) >= ALL
    (SELECT AVG(stav)
     FROM Ucet
     GROUP BY r_cislo)
;


-----------------------------------------------------
-- NEFUNGUJICI NATURAL JOIN P�I SPOJOV�N� T�� TABULEK V ORACLE 10G
SELECT * FROM Transakce;
SELECT * FROM Ucet;
SELECT * FROM Klient;

SELECT mesto, COUNT(*) AS pocet, SUM(castka) AS celkem
FROM Klient NATURAL JOIN Ucet NATURAL JOIN Transakce
WHERE datum >= TO_DATE('11.10.1998','DD.MM.YYYY') AND datum <= TO_DATE('20.10.1998','DD.MM.YYYY')
GROUP BY mesto;

SELECT mesto, COUNT(*) AS pocet, SUM(castka) AS celkem
FROM Klient K JOIN Ucet U USING (r_cislo) JOIN Transakce USING (c_uctu)
WHERE datum >= TO_DATE('11.10.1998','DD.MM.YYYY') AND datum <= TO_DATE('20.10.1998','DD.MM.YYYY')
GROUP BY mesto;

-- Po��d je asi je�t� n�jak� probl�m - viz z�vorky -> nespr�vn� v�sledek
SELECT mesto, COUNT(*) AS pocet, SUM(castka) AS celkem
FROM Klient NATURAL JOIN (Ucet NATURAL JOIN Transakce)
WHERE datum >= TO_DATE('11.10.1998','DD.MM.YYYY') AND datum <= TO_DATE('20.10.1998','DD.MM.YYYY')
GROUP BY mesto;
---------------------------------------

-- Zmena formatu data
ALTER SESSION SET NLS_DATE_FORMAT='YYYY-MM-DD';

SELECT mesto, COUNT(*) AS pocet, SUM(castka) AS celkem
FROM Klient JOIN Ucet USING (r_cislo) JOIN Transakce USING (c_uctu)
WHERE datum >= '1998-10-11' AND datum <= '1998-10-20'
GROUP BY mesto;

SELECT mesto, COUNT(*) AS pocet, SUM(castka) AS celkem
FROM Klient JOIN Ucet USING (r_cislo) JOIN Transakce USING (c_uctu)
WHERE datum >= TO_DATE('11-10-1998','DD-MM-YYYY') AND datum <= TO_DATE('20-10-1998','DD-MM-YYYY')
GROUP BY mesto;


-- Kte�� klienti maj� na sv�ch ��tech v�ce ne� 50000?
SELECT jmeno, r_cislo, SUM(stav) AS celkem
FROM Klient NATURAL JOIN Ucet
GROUP BY jmeno,r_cislo;

SELECT jmeno, r_cislo, SUM(stav) AS celkem
FROM Klient NATURAL JOIN Ucet
GROUP BY jmeno,r_cislo
HAVING SUM(stav)>50000; 

-- Nen�-li GROUP BY, chov� se jako jedna skupina
SELECT jmeno, r_cislo, SUM(stav) AS celkem
FROM Klient NATURAL JOIN Ucet
HAVING SUM(stav)>50000; 

--Tabulka s jednou hodnotou je pova�ov�na za skal�rn� hodnotu
-- Kdo m� na uctu nejvice pen�z?
SELECT * FROM Ucet;

SELECT r_cislo, jmeno, stav
FROM Klient NATURAL JOIN UCET
WHERE stav=(SELECT MAX(stav) FROM Ucet);

-- SOME,ANY,ALL
-- Kte�� mimobrn�n�t� klienti maj� na sv�ch ��tech v�ce ne� brn�n�t�?
SELECT r_cislo,jmeno,mesto FROM Klient;
INSERT INTO Ucet
VALUES(4520111,40000,'625622/6249','J�nsk�');
INSERT INTO Ucet
VALUES(4520112,40000,'625622/6249','J�nsk�');

SELECT r_cislo,jmeno,mesto FROM Klient;
SELECT * FROM Ucet;

SELECT jmeno, r_cislo, mesto, SUM(stav) AS celkem
FROM Klient NATURAL JOIN Ucet
GROUP BY jmeno, r_cislo, mesto;

SELECT jmeno, r_cislo, mesto, SUM(stav) AS celkem
FROM Klient NATURAL JOIN Ucet
WHERE mesto <> 'Brno'
GROUP BY jmeno, r_cislo, mesto
HAVING SUM(stav)>ALL
    (SELECT SUM(stav)
    FROM Klient NATURAL JOIN Ucet
    WHERE mesto='Brno'
    GROUP BY r_cislo);

-- Sloupec pou�it� pro spojen� v NATURAL JOIN nesm�
-- b�t kvalifikovan�
SELECT K.jmeno, K.r_cislo, SUM(U.stav) AS celkem
FROM Klient K NATURAL JOIN Ucet U
WHERE mesto<>'Brno'
GROUP BY jmeno,r_cislo
HAVING SUM(stav)>ALL
    (SELECT SUM(U.stav)
    FROM Klient K NATURAL JOIN Ucet U
    WHERE mesto='Brno'
    GROUP BY r_cislo);

SELECT K.jmeno, r_cislo, SUM(U.stav) AS celkem
FROM Klient K NATURAL JOIN Ucet U
WHERE mesto<>'Brno'
GROUP BY jmeno,r_cislo
HAVING SUM(stav)>ALL
    (SELECT SUM(U.stav)
    FROM Klient K NATURAL JOIN Ucet U
    WHERE mesto='Brno'
    GROUP BY r_cislo);

ROLLBACK;
-- DELETE FROM Ucet WHERE c_uctu=4520111;
-- DELETE FROM Ucet WHERE c_uctu=4520112;

-- M� n�kter� z�kazn�k ne�pln� zadanou adresu?
SELECT *
FROM Klient WHERE ulice IS NULL OR mesto IS NULL;

INSERT INTO Klient 
VALUES('491120/0423','Ji�� Nov�k',NULL,'Brno');

ROLLBACK;
--DELETE FROM Klient WHERE r_cislo='491120/0423';

-- Kte�� klienti nevlastn� u spo�itelny ��dn� ��et?
--CHYBN�
SELECT jmeno
FROM Klient NATURAL JOIN Ucet
WHERE c_uctu IS NULL;

-- SPR�VN�
SELECT jmeno
FROM Klient NATURAL LEFT JOIN Ucet
WHERE c_uctu IS NULL;

-- Kter� transakce  prob�hly v ��jnu 1998?
INSERT INTO Transakce
VALUES (4320286, 3, TO_DATE('1998-11-1','YYYY-MM-DD'), 20000);

SELECT * FROM Transakce;

SELECT jmeno, r_cislo, c_uctu, datum, castka
FROM Klient JOIN Ucet USING (r_cislo) JOIN Transakce USING (c_uctu)
WHERE datum BETWEEN TO_DATE('1998-10-1','YYYY-MM-DD') AND TO_DATE('1998-10-31','YYYY-MM-DD');

ROLLBACK;

-- EXISTS
-- Kte�� klienti maj� ��et jen u pobo�ky J�nsk�?
SELECT * FROM Ucet;

DELETE FROM Ucet WHERE c_uctu=2075752;

SELECT jmeno, c_uctu, pobocka
FROM Klient NATURAL JOIN Ucet
ORDER BY jmeno;

-- Chybn� - nekorelovan� poddotaz
SELECT DISTINCT K.*
FROM Klient K, Ucet U
WHERE K.r_cislo=U.r_cislo AND U.pobocka='J�nsk�' AND NOT EXISTS
    (SELECT * 
     FROM Ucet U, Klient K
     WHERE U.r_cislo=K.r_cislo AND U.pobocka<>'J�nsk�');

-- Spr�vn� - korelovan� poddotaz
SELECT DISTINCT K.*
FROM Klient K, Ucet U
WHERE K.r_cislo=U.r_cislo AND U.pobocka='J�nsk�' AND NOT EXISTS
    (SELECT * 
     FROM Ucet U
     WHERE U.r_cislo=K.r_cislo AND U.pobocka<>'J�nsk�');
     
-- Kte�� klienti maj� ��et u v�ech pobo�ek?
SELECT *
FROM Klient K
WHERE NOT EXISTS 
    (SELECT * FROM Pobocka P WHERE NOT EXISTS 
          (SELECT * FROM Ucet U WHERE U.r_cislo=K.r_cislo AND U.pobocka=P.nazev)
    );
   
ROLLBACK;

SELECT * FROM Transakce;

-- Kte�� klienti maj� k�estn� jm�no Jan?
SELECT jmeno FROM Klient;

SELECT *
FROM Klient
WHERE jmeno LIKE 'Jan %';

-- Kte�� klienti maj� u pobo�ky J�nsk� jen jeden ��et?
INSERT INTO Ucet
VALUES(1182649,10853,'530610/4532','J�nsk�');

SELECT r_cislo,jmeno
FROM Klient NATURAL JOIN Ucet
WHERE Pobocka='J�nsk�';

SELECT K.*
FROM Klient K, Ucet U
WHERE K.r_cislo=U.r_cislo AND U.pobocka='J�nsk�' 
   AND (SELECT COUNT(*)
       FROM Ucet U
       WHERE K.r_cislo=U.r_cislo AND
         U.pobocka='J�nsk�') = 1;
-- Jin� varianta
SELECT K.*
FROM Klient K, Ucet U
WHERE K.r_cislo=U.r_cislo AND U.pobocka='J�nsk�' 
  AND NOT EXISTS
    (SELECT *
     FROM Ucet U1
     WHERE K.r_cislo=U1.r_cislo AND
       U1.pobocka='J�nsk�' AND U.c_uctu <> U1.c_uctu);

ROLLBACK;

-- Kte�� klienti jsou z Brna nebo Prahy?
SELECT *
FROM Klient
WHERE mesto IN ('Praha', 'Brno');

-- Kte�� klienti prov�d�li transakce v ��jnu 1998?
SELECT jmeno, r_cislo, c_uctu, datum, castka
FROM Klient JOIN Ucet USING (r_cislo) JOIN Transakce USING (c_uctu)
WHERE datum BETWEEN TO_DATE('1998-10-1','YYYY-MM-DD') 
                    AND TO_DATE('1998-10-31','YYYY-MM-DD');

SELECT DISTINCT jmeno, r_cislo
FROM Klient JOIN Ucet USING (r_cislo) JOIN Transakce USING (c_uctu)
WHERE datum BETWEEN TO_DATE('1998-10-1','YYYY-MM-DD') 
                    AND TO_DATE('1998-10-31','YYYY-MM-DD');

SELECT jmeno, r_cislo
FROM Klient
WHERE r_cislo IN
   (SELECT r_cislo FROM Ucet
    WHERE c_uctu IN
     (SELECT c_uctu  FROM Transakce
      WHERE datum BETWEEN TO_DATE('1998-10-1','YYYY-MM-DD') 
                      AND TO_DATE('1998-10-31','YYYY-MM-DD')));
SELECT * FROM Transakce;
SELECT * FROM Ucet;

-- Kte�� klienti maj� u pobo�ky J�nsk� ��et nebo p�j�ku?
CREATE TABLE Pujcka (
c_pujcky  INTEGER NOT NULL,
r_cislo CHAR(11) NOT NULL,
pobocka VARCHAR(20),
castka DECIMAL(7,2),
splaceno DECIMAL(7,2),
PRIMARY KEY (c_pujcky),
FOREIGN KEY (r_cislo) REFERENCES Klient ON DELETE CASCADE,
FOREIGN KEY (pobocka) REFERENCES Pobocka
); 

INSERT INTO Pujcka
VALUES(111111,'580807/9638','J�nsk�',60000,10000);

SELECT jmeno, r_cislo, c_uctu, pobocka FROM Klient NATURAL JOIN Ucet WHERE pobocka='J�nsk�';
SELECT jmeno, r_cislo, c_pujcky, pobocka FROM Klient NATURAL JOIN Pujcka WHERE pobocka='J�nsk�';

-- Vlozeni dalsiho uctu (kvuli ALL)
INSERT INTO Ucet
VALUES(2348532,10000,'530610/4532','J�nsk�');

SELECT jmeno, r_cislo, c_uctu, pobocka FROM Klient NATURAL JOIN Ucet WHERE pobocka='J�nsk�';
SELECT jmeno, r_cislo, c_pujcky, pobocka FROM Klient NATURAL JOIN Pujcka WHERE pobocka='J�nsk�';


SELECT jmeno, r_cislo
FROM Klient NATURAL JOIN Ucet
WHERE pobocka='J�nsk�'
UNION
SELECT  jmeno, r_cislo
FROM Klient NATURAL JOIN Pujcka
WHERE pobocka='J�nsk�';

SELECT jmeno, r_cislo
FROM Klient NATURAL JOIN Ucet
WHERE pobocka='J�nsk�'
UNION ALL
SELECT  jmeno, r_cislo
FROM Klient NATURAL JOIN Pujcka
WHERE pobocka='J�nsk�';

ROLLBACK;
DROP TABLE Pujcka CASCADE CONSTRAINTS;

-- Pou�it� pseudosloupce ROWNUM pro omezen� v�b�r
SELECT *
FROM Klient
WHERE ROWNUM < 5;

SELECT *
FROM Klient
WHERE ROWNUM < 5
ORDER BY jmeno;

SELECT *
FROM (SELECT * FROM Klient ORDER BY jmeno)
WHERE ROWNUM < 5;

SELECT * FROM Ucet;
SELECT * FROM Klient;
SELECT * FROM Transakce;

