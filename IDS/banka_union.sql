-- Uk�zka eliminace duplicit u mno�inov�ch operac�
SELECT * FROM Klient;
SELECT * FROM Ucet;
SELECT mesto FROM Klient;

SELECT mesto FROM Klient
UNION
SELECT mesto FROM Klient;

SELECT mesto FROM Klient
UNION ALL
SELECT mesto FROM Klient;

SELECT DISTINCT mesto FROM Klient
UNION ALL
SELECT DISTINCT mesto FROM Klient;

SELECT DISTINCT mesto FROM Klient
UNION ALL
SELECT mesto FROM Klient;

SELECT mesto FROM Klient
INTERSECT
SELECT mesto FROM Klient;

SELECT mesto FROM Klient
MINUS
SELECT mesto FROM Klient;

SELECT r_cislo,pobocka FROM Klient NATURAL JOIN Ucet;

SELECT r_cislo FROM Klient NATURAL JOIN Ucet
   WHERE pobocka = 'J�nsk�'
MINUS
SELECT r_cislo FROM Klient NATURAL JOIN Ucet
   WHERE pobocka <>'J�nsk�';

DELETE FROM Ucet WHERE r_cislo = '530610/4532' AND pobocka = 'Palack�ho';
ROLLBACK;