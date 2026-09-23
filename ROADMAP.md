# Roadmap

## 目标

```sql
cdb shell

cdb> CREATE TABLE student (
       id INT,
       name TEXT,
       score INT
     );

cdb> INSERT INTO student VALUES (1, "Alice", 95);
cdb> INSERT INTO student VALUES (2, "Bob", 87);

cdb> SELECT * FROM student;
cdb> SELECT name, score FROM student WHERE id = 1;

cdb> CREATE INDEX idx_student_id ON student(id);
cdb> DELETE FROM student WHERE id = 2;
cdb> UPDATE student SET score = 90 WHERE id = 2;
```

