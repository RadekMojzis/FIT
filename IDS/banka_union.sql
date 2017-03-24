-- Ukázka eliminace duplicit u množinových operací
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
   WHERE pobocka = 'Jánská'
MINUS
SELECT r_cislo FROM Klient NATURAL JOIN Ucet
   WHERE pobocka <>'Jánská';

DELETE FROM Ucet WHERE r_cislo = '530610/4532' AND pobocka = 'Palackého';
ROLLBACK;