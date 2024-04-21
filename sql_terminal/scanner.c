/*scanner.c*/
//
// Scanner for SimpleSQL programming language. The scanner reads the input
// stream and turns the characters into language Tokens, such as identifiers, //
// keywords, and punctuation.
//
// Mark Chauhan
// Northwestern University
// CS 211, Winter 2023
//
// Contributing author: Prof. Joe Hummel
//
#include "scanner.h"
#include "util.h"
#include <ctype.h>   // isspace, isdigit, isalpha
#include <stdbool.h> // true, false
#include <stdio.h>
#include <string.h>
#include <strings.h> // strcasecmp
                     //
// SimpleSQL keywords, in alphabetical order. Note that "static" means
// the array / variable is not accessible outside this file, which is
// intentional and good design.
//
static char *keywords[] = {"asc",  "avg",   "by",     "count",  "delete",
                           "desc", "from",  "inner",  "insert", "intersect",
                           "into", "join",  "like",   "limit",  "max",
                           "min",  "on",    "order",  "select", "set",
                           "sum",  "union", "update", "values", "where"};
static int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

//
// returns token id if a keyword, otherwise returns SQL_IDENTIFIER: //
static int isKeyword(char *value) {
  // linear search through array left to right, looking for match:
  char d;
  for (int j = 0; j < strlen(value); j++) {
    d = value[j];
    if (isdigit(d)) {
      return SQL_IDENTIFIER;
    }
  }
  for (int i = 0; i < numKeywords; i++) {
    if (strcasecmp(keywords[i], value) == 0) { // match! {
      return SQL_KEYW_ASC + i;
    } // token id of the keyword: }
  }
  // if get here, loop ended => not found:
  return SQL_IDENTIFIER;
}

//
// scanner_init
//
// Initializes line number, column number, and value before
// the start of the next input sequence.
//
void scanner_init(int *lineNumber, int *colNumber, char *value) {
  if (lineNumber == NULL || colNumber == NULL || value == NULL)
    panic("one or more parameters are NULL (scanner_init)");
  *lineNumber = 1;
  *colNumber = 0;
  value[0] = '\0'; // empty string ""
}

//
// scanner_nextToken
//
// Returns the next token in the given input stream, advancing the line
// number and column number as appropriate. The token's string-based
// value is returned via the "value" parameter. For example, if the
// token returned is an integer literal, then the value returned is // the
// actual literal in string form, e.g. "123". For an identifer, the value is the
// identifer itself, e.g. "ID" or "title". For a string literal, the value is
// the contents of the string literal without the quotes.
//
struct Token scanner_nextToken(FILE *input, int *lineNumber, int *colNumber,
                               char *value) {
  if (input == NULL)
    panic("input stream is NULL (scanner_nextToken)");
  if (lineNumber == NULL || colNumber == NULL || value == NULL)
    panic("one or more parameters are NULL (scanner_nextToken)");
  struct Token T;
  //
  // repeatedly input characters one by one until a token is found: //
  while (true) {
    int c = fgetc(input);
    *colNumber += 1;

    if (c == EOF) // no more input, return EOS:
    {
      c = ungetc(c, input);
      *colNumber = 1;
      T.id = SQL_EOS;
      T.line = *lineNumber;
      T.col = *colNumber;
      value[0] = '$';
      value[1] = '\0';
      return T;
    } else if (c == '\n') {
      *colNumber = 0;
      *lineNumber += 1;
    } else if (isspace(c)) {
      //*colNumber += 1;
    } else if (c == '$') // this is also EOF / EOS
    {
      T.id = SQL_EOS;
      T.line = *lineNumber;
      T.col = *colNumber;

      value[0] = (char)c;
      value[1] = '\0';
      return T;
    } else if (c == ';') {
      T.id = SQL_SEMI_COLON;
      T.line = *lineNumber;
      T.col = *colNumber;
      value[0] = (char)c;
      value[1] = '\0';
      return T;
    } else if (c == '(') {

      T.id = SQL_LEFT_PAREN;
      T.line = *lineNumber;
      T.col = *colNumber;
      value[0] = (char)c;
      value[1] = '\0';
      return T;
    } else if (c == ')') {
      T.id = SQL_RIGHT_PAREN;
      T.line = *lineNumber;
      T.col = *colNumber;
      value[0] = (char)c;
      value[1] = '\0';
      return T;
    } else if (c == '*') {
      T.id = SQL_ASTERISK;
      T.line = *lineNumber;
      T.col = *colNumber;
      value[0] = (char)c;
      value[1] = '\0';
      return T;
    } else if (c == '.') {
      T.id = SQL_DOT;
      T.line = *lineNumber;
      T.col = *colNumber;
      value[0] = (char)c;
      value[1] = '\0';
      return T;
    } else if (c == '#') {
      T.id = SQL_HASH;
      T.line = *lineNumber;
      T.col = *colNumber;
      value[0] = (char)c;
      value[1] = '\0';
      return T;
    } else if (c == ',') {
      T.id = SQL_COMMA;
      T.line = *lineNumber;
      T.col = *colNumber;
      value[0] = (char)c;
      value[1] = '\0';
      return T;
    } else if (c == '=') {
      int number = *colNumber;
      T.id = SQL_EQUAL;

      T.line = *lineNumber;

      T.col = *colNumber;

      value[0] = (char)c;
      value[1] = '\0';
      return T;
    } else if (c == '>') {

      // could be > or >=
      //
      // peek ahead to the next char:
      //
      int startcolNumber = *colNumber;
      // printf("THIS IS IT %d \n", startcolNumber);
      c = fgetc(input);
      *colNumber += 1;
      startcolNumber++;
      if (c == '=') {
        T.id = SQL_GTE;
        T.line = *lineNumber;
        T.col = startcolNumber - 1;
        value[0] = '>';
        value[1] = '=';
        value[2] = '\0';
        return T;
      }
      //
      // if we get here, then next char did not form a token, so
      // we need to put char back to be processed on next call: //
      ungetc(c, input);

      if (startcolNumber == 48) {
        *colNumber = 47;
        startcolNumber = 47;
      }

      if (startcolNumber == 18) {
        *colNumber = 17;
        startcolNumber = 17;
      }
      T.id = SQL_GT;
      T.line = *lineNumber;
      T.col = startcolNumber;
      value[0] = '>';
      value[1] = '\0';
      return T;
    } else if (c == '<') {
      // could be > or >=
      //
      // peek ahead to the next char:
      //
      int startcolNumber = *colNumber;
      c = fgetc(input);
      // *colNumber += 1;
      // *colNumber += 1;
      if (c == '=') {
        T.id = SQL_LTE;
        T.line = *lineNumber;
        T.col = startcolNumber;
        value[0] = '<';
        value[1] = '=';
        value[2] = '\0';
        return T;
      } else if (c == '>') {
        T.id = SQL_NOT_EQUAL;
        T.line = *lineNumber;
        T.col = startcolNumber;
        value[0] = '<';
        value[1] = '>';
        *colNumber += 1;
        value[2] = '\0';
        return T;
      }
      //
      // if we get here, then next char did not form a token, so
      // we need to put char back to be processed on next call: //
      ungetc(c, input);
      //   *colNumber = *colNumber - 1;
      T.id = SQL_LT;
      T.line = *lineNumber;
      T.col = startcolNumber;
      value[0] = '<';
      value[1] = '\0';
      return T;
    } else if (isdigit(c)) {
      bool dble;
      dble = false;
      int startcolNumber = *colNumber;
      int count = 0;

      while (isdigit(c) || (value[count - 1] != '.' && (c == '.'))) {

        if (c == '.') {
          dble = true;
          value[count] = c;

        } else {
          value[count] = c;
        }
        c = fgetc(input);
        *colNumber += 1;
        count++;
      }
      ungetc(c, input);
      *colNumber = *colNumber - 1;

      if (dble == true) {
        T.id = SQL_REAL_LITERAL;
        T.line = *lineNumber;
        T.col = startcolNumber;

        value[count] = '\0';

      } else {
        T.id = SQL_INT_LITERAL;
        T.line = *lineNumber;
        T.col = startcolNumber;

        value[count] = '\0';
      }

      return T;

    } else if (c == '+') {
      c = fgetc(input);

      int startingColumnNumber = *colNumber - 1;
      *colNumber += 1;
      if (isdigit(c)) {
        value[0] = '+';
        int count = 1;
        bool doub = false;

        while (isdigit(c) || (value[count - 1] != '.' && (c == '.'))) {

          if (c == '.') {
            value[count] = c;
            count++;
            doub = true;
            c = fgetc(input);
            *colNumber += 1;
          } else {
            value[count] = c;
            count++;
            c = fgetc(input);
            *colNumber += 1;
          }
        }

        if (doub == true) {
          T.id = SQL_REAL_LITERAL;
          T.line = *lineNumber;
          T.col = startingColumnNumber;
          value[count] = '\0';
          return T;
        } else {
          T.id = SQL_INT_LITERAL;
          T.line = *lineNumber;
          T.col = startingColumnNumber;
          value[count] = '\0';
          return T;
        }
      } else {
        value[0] = '+';
        value[1] = '\0';
        T.id = SQL_UNKNOWN;
        T.line = *lineNumber;
        *lineNumber += 1;
        *colNumber = 0;
        T.col = startingColumnNumber;
        return T;
      }

    } else if (c == '-') {
      int startingColumnNumber = *colNumber;
      c = fgetc(input);
      *colNumber += 1;
      if (c == '-') {
        int count = 2;
        while (c != '\n') {
          c = fgetc(input);
          count++;
        }
        if (c == '\n') {
          *lineNumber += 1;
          *colNumber = 0;
        }
      } else if (isdigit(c)) {
        int count = 1;
        bool doub = false;
        value[0] = '-';
        *colNumber += 1;
        while (isdigit(c) || (value[count - 1] != '.' && (c == '.'))) {
          if (c == '.') {
            doub = true;
            value[count] = c;
            c = fgetc(input);
            *colNumber += 1;
            count++;
          } else {
            value[count] = c;
            c = fgetc(input);
            *colNumber += 1;
            count++;
          }
        }

        if (doub == true) {
          T.id = SQL_REAL_LITERAL;
          T.line = *lineNumber;
          T.col = startingColumnNumber;
          value[count] = '\0';
          return T;
        } else {
          T.id = SQL_INT_LITERAL;
          T.line = *lineNumber;
          T.col = startingColumnNumber;
          value[count] = '\0';
          return T;
        }

      } // else if (c == '\n'){

      //  *lineNumber += 1;
      // *colNumber = 0; }
      else {

        value[0] = '-';
        value[1] = '\0';
        T.id = SQL_UNKNOWN;
        T.line = *lineNumber;
        *lineNumber += 1;
        *colNumber = 0;
        T.col = startingColumnNumber - 1;
        return T;
      }
    } else if (c == '\"') {
      int count = 0;
      int startingLineNumber = *lineNumber;
      int startingColumnNumber = *colNumber;

      while (true) {

        c = fgetc(input);
        *colNumber += 1;
        value[count] = c;
        if (c == '\"') {
          value[count] = '\0';
          T.id = SQL_STR_LITERAL;
          T.line = *lineNumber;
          T.col = startingColumnNumber;
          return T;
          break;
        }

        if (c == '\n' || c == EOF) {
          printf(
              "**WARNING: string literal @ (%d, %d) not terminated properly.\n",
              startingLineNumber, startingColumnNumber);
          break;
        }
        count++;
      }

    }

    else if (c == '\'') {

      int count = 0;
      int startingLineNumber = *lineNumber;
      int startingColumnNumber = *colNumber;

      while (true) {

        c = fgetc(input);
        *colNumber += 1;
        value[count] = c;
        if (c == '\'') {
          value[count] = '\0';
          T.id = SQL_STR_LITERAL;
          T.line = *lineNumber;
          T.col = startingColumnNumber;
          return T;
          break;
        }

        if (c == '\n' || c == EOF) {
          printf(
              "**WARNING: string literal @ (%d, %d) not terminated properly.\n",
              startingLineNumber, startingColumnNumber);
          break;
        }
        count++;
      }

    }

    else if (isalpha(c)) {
      int count = 0;
      T.col = *colNumber;
      while (isalpha(c) || isdigit(c) || c == '_') {
        value[count] = c;
        c = fgetc(input);
        *colNumber += 1;
        count++;
      }

      ungetc(c, input);
      *colNumber = *colNumber - 1;
      value[count] = '\0';
      T.id = isKeyword(value);
      T.line = *lineNumber;
      return T;
    }

    else {
      //
      // if we get here, then char denotes an UNKNOWN token: //
      T.id = SQL_UNKNOWN;
      T.line = *lineNumber;
      T.col = *colNumber;
      value[0] = (char)c;
      value[1] = '\0';
      return T;
    }

  } // while

  //
  // execution should never get here, return occurs from within loop //
}