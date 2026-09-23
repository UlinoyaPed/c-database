# Architecture

```
SQL String
  |
Lexer 词法分析
  |
Parser 语法分析
  |
AST
  |
Exec
  |
B+ Tree
  |
Record Layer
  |
Page Manager
  |
Pager
Cache
  |
database.db
```

## Lexer

`Lexical Analyzer` 词法分析，将 SQL 语句转化为可处理的 TOKEN

`String` -> `TOKENS`

输入：

```sql
SELECT name, score FROM student WHERE id = 1001;
```

输出：

```
SELECT;
IDENTIFER("name");
COMMA;
IDENTIFER("score");
FROM;
IDENTIFER("student");
WHERE;
IDENTIFER("id");
EQUAL;
NUMBER(1001);
SEMICOLON;
EOF
```
