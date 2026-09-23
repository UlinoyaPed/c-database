#pragma once

#include <stddef.h>

typedef enum CDBTokenType {
  CDB_TOKEN_EOF,
  CDB_TOKEN_ERROR,

  // 关键字
  CDB_TOKEN_SELECT,
  CDB_TOKEN_FROM,
  CDB_TOKEN_WHERE,
  CDB_TOKEN_INSERT,
  CDB_TOKEN_INTO,
  CDB_TOKEN_VALUES,
  CDB_TOKEN_CREATE,
  CDB_TOKEN_TABLE,
  CDB_TOKEN_UPDATE,
  CDB_TOKEN_DELETE,

  // 字面量
  CDB_TOKEN_IDENTIFIER,
  CDB_TOKEN_NUMBER,
  CDB_TOKEN_STRING,

  // 运算符
  CDB_TOKEN_STAR,
  CDB_TOKEN_SLASH,
  CDB_TOKEN_PLUS,
  CDB_TOKEN_MINUS,

  CDB_TOKEN_EQUAL,
  CDB_TOKEN_NOT_EQUAL,
  CDB_TOKEN_LESS,
  CDB_TOKEN_LESS_EQUAL,
  CDB_TOKEN_GREATER,
  CDB_TOKEN_GREATER_EQUAL,

  // 标点
  CDB_TOKEN_COMMA,
  CDB_TOKEN_SEMICOLON,
  CDB_TOKEN_L_PAREN,
  CDB_TOKEN_R_PAREN,
} CDBTokenType;

typedef struct CDBToken { // Token
  CDBTokenType type;

  const char *start;
  size_t length;
} CDBToken;

typedef struct CDBLexer { // Lexer 扫描器
  // 整个 SQL
  const char *source;
  size_t length;

  size_t start;   // 当前 token 开始位置
  size_t current; // 已经读到位置
} CDBLexer;

void CDBInitLexer(CDBLexer *lexer, const char *source);

CDBToken CDBLexerNextToken(CDBLexer *lexer);

// TODO:
CDBToken CDBLexerPeekNext(CDBLexer *lexer);