-- Pou�it� uvozovek pro �e�en� kolize identifik�tor� s rezervovan�mi
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