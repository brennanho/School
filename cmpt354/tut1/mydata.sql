PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE t1(name text, surname text);
INSERT INTO t1 VALUES('bob',NULL);
COMMIT;