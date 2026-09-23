#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief 构建 token
 * @n 截取 source[start ~ current]
 *
 * @param type token 的类型
 * @return 构建的 token
 */
static CDBToken makeToken(const CDBLexer *lexer, CDBTokenType type) {
  CDBToken token;
  token.type = type;
  token.start = lexer->source + lexer->start;
  token.length = lexer->current - lexer->start;

  return token;
}

/**
 * @brief 构建包含错误信息的 token
 *
 * @param message 错误信息
 * @return 构建的 token
 */
static CDBToken makeErrorToken(const char *message) {
  CDBToken token;
  token.type = CDB_TOKEN_ERROR;
  token.start = message;
  token.length = strlen(message);
  return token;
}

/**
 * @brief 判断是否到达字符串末尾
 */
static bool isAtEnd(const CDBLexer *lexer) {
  return lexer->current >= lexer->length;
}

/**
 * @brief 查看当前字符，若已达到末尾则返回 `\0`
 *
 * @return 当前字符
 */
static char peekCurrent(const CDBLexer *lexer) {
  if (isAtEnd(lexer)) {
    return '\0';
  }

  return lexer->source[lexer->current];
}

/**
 * @brief 查看下一个字符，若没有则返回 `\0`
 *
 * @return 下一个字符
 */
static char peekNext(const CDBLexer *lexer) {
  if (lexer->current + 1 >= lexer->length) {
    return '\0';
  }
  return lexer->source[lexer->current + 1];
}

/**
 * @brief 判断当前读取的字符是否等于 expected
 *
 * @param expected 期望的字符
 */
static bool match(const CDBLexer *lexer, char expected) {
  if (isAtEnd(lexer))
    return false;

  if (lexer->source[lexer->current] != expected)
    return false;

  return true;
}

/**
 * @brief 读取**当前**扫描的字符，current 指针后移 1
 *
 * @param lexer mut(current): current++
 *
 * @return 当前字符
 */
static char advanceMut(CDBLexer *lexer) {
  return lexer->source[lexer->current++];
}

/**
 * @brief 连续查看下一个字符，若为空字符则跳过
 * @n 函数结束时 current 不为 whitespace，但可能是 `\0`
 *
 * @param lexer mut(current): 调用 advance, current 增至 white space 后
 */
static void skipWhitespaceMut(CDBLexer *lexer) {
  while (true) {
    char c = peekNext(lexer);
    switch (c) {
    case ' ':
    case '\n':
    case '\t':
      advanceMut(lexer); // mut: current
      break;
    // TODO: SQL注释
    case '-':
      break;
    default:
      return;
    }
  }
}

/**
 * @brief 判断一个字符是否可以作为标识符的开头
 *
 * @note Identifier 例如表名、列名 的开头只能是字母
 */
static bool isValidIdentifierStart(char c) {
  return isalpha((unsigned char)c) || c == '_';
}

/**
 * @brief 判断一个字符是否可以作为标识符的中间部分
 */
static bool isValidIdentifierPart(char c) {
  return isalnum((unsigned char)c) || c == '_'; // 字母、数字或下划线
}

/**
 * @brief 判断 `lexer` 当前读取的字符串是否等于 `keyword`
 *
 * @param keyword 目标字符串
 */
static bool compareKeyword(const CDBLexer *lexer, const char *keyword) {
  size_t keyword_length = strlen(keyword);
  size_t token_length = lexer->current - lexer->start;

  if (keyword_length != token_length)
    return false;

  for (size_t i = 0; i < keyword_length; ++i) {
    if (toupper(lexer->source[lexer->start + i]) != keyword[i]) {
      return false;
    }
  }

  return true;
}

static CDBTokenType getIdentifierType(const CDBLexer *lexer) {
  if (compareKeyword(lexer, "SELECT"))
    return CDB_TOKEN_SELECT;
  // TODO: 完善其他类型的匹配

  return CDB_TOKEN_IDENTIFIER;
}

/**
 * @brief 制作标识符 token
 * @n 需要 start 指向 Identifier 的开头
 * @n 持续读到 Identifier 结尾，返回整个 Identifier token
 * @n 函数退出时 current 指向 Identifier 末位
 *
 * @param lexer mut(current): 调用 advance, current 增至 Identifier end
 */
static CDBToken makeIdentifierTokenMut(CDBLexer *lexer) {
  while (isValidIdentifierPart(peekNext(lexer))) { // 使用的是 peekNext
    advanceMut(lexer);                             // mut
  }
  // current 指向 Identifier 末位

  return makeToken(lexer, getIdentifierType(lexer));
}

/**
 * @brief 制作数字 token
 *
 * @n 需要 start 指向 Number 的开头
 * @n 持续读到 Number 结尾，返回整个 Number token
 * @n 函数退出时 current 指向 Number 末位
 *
 * @param lexer mut(current): 调用 advance, current 增至 Identifier end
 */
static CDBToken makeNumberTokenMut(CDBLexer *lexer) {
  while (isdigit((unsigned char)peekNext(lexer))) {
    advanceMut(lexer);
  }

  // TODO: 支持小数

  return makeToken(lexer, CDB_TOKEN_NUMBER);
}

/**
 * @brief 初始化 lexer
 *
 * @param lexer mut: 初始化 `source`, `length`, `start`, `current`
 * @param source SQL 字符串
 */
void CDBInitLexer(CDBLexer *lexer, const char *source) {
  lexer->source = source;
  lexer->length = strlen(source);
  lexer->start = 0;
  lexer->current = 0;
}

/**
 * @brief 逐步遍历 token
 * @n
 *
 * @param lexer mut: 修改 `start`, `current`
 *
 * @return 接下来的 token
 * @n 如果到达末尾则 type 为 EOF
 */
CDBToken CDBLexerNextToken(CDBLexer *lexer) {
  skipWhitespaceMut(lexer);

  lexer->start = lexer->current;

  if (isAtEnd(lexer))
    return makeToken(lexer, CDB_TOKEN_EOF);

  char c = advanceMut(lexer); // c = source[current]
  // current = start + 1

  // 如果匹配 Identifier 则制作 Identifier token
  if (isValidIdentifierStart(c)) {
    return makeIdentifierTokenMut(lexer);
  }

  // 匹配数字
  if (isdigit((unsigned char)c)) {
    return makeNumberTokenMut(lexer);
  }

  // 匹配运算符
  switch (c) {
  case '*':
    return makeToken(lexer, CDB_TOKEN_STAR);
  case '/':
    return makeToken(lexer, CDB_TOKEN_SLASH);
  case '+':
    return makeToken(lexer, CDB_TOKEN_PLUS);
  case '-':
    return makeToken(lexer, CDB_TOKEN_MINUS);

  case '=':
    return makeToken(lexer, CDB_TOKEN_EQUAL);
  case '!':
    if (peekNext(lexer) == '=') {
      advanceMut(lexer);
      return makeToken(lexer, CDB_TOKEN_NOT_EQUAL);
    }
    return makeErrorToken("expected = after !");
  //
  case '>':
    if (peekNext(lexer) == '=') {
      advanceMut(lexer);
      return makeToken(lexer, CDB_TOKEN_GREATER_EQUAL);
    } else {
      return makeToken(lexer, CDB_TOKEN_GREATER);
    }
  case '<':
    if (peekNext(lexer) == '=') {
      advanceMut(lexer);
      return makeToken(lexer, CDB_TOKEN_LESS_EQUAL);
    } else {
      return makeToken(lexer, CDB_TOKEN_LESS);
    }
  case ',':
    return makeToken(lexer, CDB_TOKEN_COMMA);
  case ';':
    return makeToken(lexer, CDB_TOKEN_SEMICOLON);
  case '(':
    return makeToken(lexer, CDB_TOKEN_L_PAREN);
  case ')':
    return makeToken(lexer, CDB_TOKEN_R_PAREN);
  }
}
