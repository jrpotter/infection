-- Note I did not focus on portability in terms of table names and
-- fields. Instead, I'm providing a basic SQL template for anyone
-- interested in adding their own test cases. To do this, copy the following
-- segment and revise the inserted values.

-- ============================ COPY START ============================ --

PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS student;
DROP TABLE IF EXISTS j_student;

-- Represents the student and the version they are on
CREATE TABLE student (
    id INT PRIMARY KEY     NOT NULL,
    name           TEXT    NOT NULL,
    version        INT     NOT NULL
);

-- Allows the many to many relationship possible between students
CREATE TABLE j_student (
    mentor      INTEGER,
    mentee      INTEGER
);

-- ============================ COPY END ============================ --

-- This is where to create your own test cases. Note this only needs to be called once, and multiple
-- databases can be created with a variety of information.

INSERT INTO student (id,name,version) VALUES (1, 'Paul', 1);
INSERT INTO student (id,name,version) VALUES (2, 'Allen', 1);
INSERT INTO student (id,name,version) VALUES (3, 'Teddy', 1);
INSERT INTO student (id,name,version) VALUES (4, 'Mark', 2);
INSERT INTO student (id,name,version) VALUES (5, 'Chase', 1);
INSERT INTO student (id,name,version) VALUES (6, 'Aaron', 1);
INSERT INTO student (id,name,version) VALUES (7, 'Morgan', 1);
INSERT INTO student (id,name,version) VALUES (8, 'Nancy', 3);
INSERT INTO student (id,name,version) VALUES (9, 'Joshua', 1);
INSERT INTO student (id,name,version) VALUES (10, 'Justin', 3);

INSERT INTO j_student (mentor, mentee) VALUES(1, 2);
INSERT INTO j_student (mentor, mentee) VALUES(2, 3);
INSERT INTO j_student (mentor, mentee) VALUES(2, 4);
INSERT INTO j_student (mentor, mentee) VALUES(3, 1);
INSERT INTO j_student (mentor, mentee) VALUES(4, 3);
INSERT INTO j_student (mentor, mentee) VALUES(4, 5);
INSERT INTO j_student (mentor, mentee) VALUES(4, 6);
INSERT INTO j_student (mentor, mentee) VALUES(5, 6);
INSERT INTO j_student (mentor, mentee) VALUES(6, 7);
INSERT INTO j_student (mentor, mentee) VALUES(7, 5);
INSERT INTO j_student (mentor, mentee) VALUES(7, 8);
INSERT INTO j_student (mentor, mentee) VALUES(7, 9);
INSERT INTO j_student (mentor, mentee) VALUES(8, 9);
INSERT INTO j_student (mentor, mentee) VALUES(9, 10);
INSERT INTO j_student (mentor, mentee) VALUES(10, 8);

