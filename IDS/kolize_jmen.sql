-- Použití uvozovek pro øešení kolize identifikátorù s rezervovanými
SELECT table_name FROM TABS;

DROP TABLE "TABLE";

CREATE TABLE TABLE (
a INTEGER);

CREATE TABLE "TABLE" (
a INTEGER);

SELECT a
FROM TABLE;

SELECT a
FROM "TABLE";

INSERT INTO "TABLE" VALUES (1);