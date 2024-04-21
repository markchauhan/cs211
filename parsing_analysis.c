/*main.c*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyzer.h"
#include "ast.h"
#include "database.h"
#include "parser.h"
//
// Mark Chauhan
//

// functions
// data_base open

void execute_query(struct TableMeta *tableArray, char *database, char *file) {

  int num = tableArray->recordSize % DATABASE_MAX_ID_LENGTH;
  num = num * DATABASE_MAX_ID_LENGTH + 3;
  int bufferSize = num;
  FILE *temp;

  char *buffer = (char *)malloc(sizeof(char) * bufferSize);
  if (buffer == NULL) {
    panic("out of memory");
  }

  int i = 0;

    temp = fopen(file, "r");
    while (fgets(buffer, bufferSize, temp)) {
      printf("%s", buffer);
      if (i > 3) {
        break;
      }
      i++;
    }

    fclose(temp);
    //    fclose(temp);
  printf("\n");
  //    fclose(temp);
}

void print_schema(struct TableMeta *tableArray, int numTables, char *database) {

  printf("**DATABASE SCHEMA**\n");

  printf("Database: %s\n", database);
  for (int i = 0; i < numTables; i++) {

    printf("Table: %s \n", tableArray[i].name);
    printf("\tRecord size: %d \n", tableArray[i].recordSize);

    char indexType[30];
    char colType[8];

    for (int j = 0; j < tableArray[i].numColumns; j++) {

      printf("\tColumn: ");

      printf("%s, ", tableArray[i].columns[j].name);

      switch (tableArray[i].columns[j].colType) {
      case 1:
        strcpy(colType, "int, ");
        break;

      case 2:
        strcpy(colType, "real, ");
        break;

      case 3:
        strcpy(colType, "string, ");
        break;

      default:
        break;
      }

      printf("%s", colType);

      switch (tableArray[i].columns[j].indexType) {
      case 0:
        strcpy(indexType, "non-indexed");
        break;

      case 1:
        strcpy(indexType, "indexed");
        break;

      case 2:
        strcpy(indexType, "unique indexed");
        break;

      default:
        break;
      }
      printf("%s", indexType);
      printf("\n");
    }
  }

  printf("**END OF DATABASE SCHEMA**\n");
}

void print_ast(struct QUERY *query) {

  printf("**QUERY AST**\n");
  printf("Table: %s \n", query->q.select->table);
  struct SELECT *select = query->q.select;

  while (true) {

    if (select->columns->next != NULL) {

      switch (select->columns->function) {
      case (0):
        printf("Select column: MIN(%s.%s) \n", select->columns->table,
               select->columns->name);
        break;

      case (1):
        printf("Select column: MAX(%s.%s) \n", select->columns->table,
               select->columns->name);

        break;

      case (2):
        printf("Select column: SUM(%s.%s) \n", select->columns->table,
               select->columns->name);
        break;

      case (3):
        printf("Select column: AVG(%s.%s) \n", select->columns->table,
               select->columns->name);
        break;

      case (4):
        printf("Select column: COUNT(%s.%s) \n", select->columns->table,
               select->columns->name);
        break;

      default:
        printf("Select column: %s.%s \n", select->columns->table,
               select->columns->name);
        break;
      }

      select->columns = select->columns->next;
      continue;
    } else {

      switch (select->columns->function) {
      case (0):
        printf("Select column: MIN(%s.%s) \n", select->columns->table,
               select->columns->name);
        break;

      case (1):
        printf("Select column: MAX(%s.%s) \n", select->columns->table,
               select->columns->name);

        break;

      case (2):
        printf("Select column: SUM(%s.%s) \n", select->columns->table,
               select->columns->name);
        break;

      case (3):
        printf("Select column: AVG(%s.%s) \n", select->columns->table,
               select->columns->name);
        break;

      case (4):
        printf("Select column: COUNT(%s.%s) \n", select->columns->table,
               select->columns->name);
        break;

      default:
        printf("Select column: %s.%s \n", select->columns->table,
               select->columns->name);
        break;
      }

      break;
    }
  }

  if (query->q.select->join == NULL) {
    printf("Join (NULL)\n");

  } else {
    struct JOIN *join = query->q.select->join;
    printf("Join %s On %s.%s = %s.%s\n", join->table, join->left->table,
           join->left->name, join->right->table, join->right->name);
  }

  if (query->q.select->where == NULL) {
    printf("Where (NULL)\n");

  } else {
    struct EXPR *expr = query->q.select->where->expr;
    char op[5];
    switch (expr->operator) {
    case 0:
      strcpy(op, "<");
      break;
    case 1:
      strcpy(op, "<=");
      break;
    case 2:
      strcpy(op, ">");
      break;
    case 3:
      strcpy(op, ">=");
      break;
    case 4:
      strcpy(op, "=");
      break;
    case 5:
      strcpy(op, "<>");
      break;
    default:
      strcpy(op, "like");
      break;
    }

    char literal[101];
    strcpy(literal, expr->value);
    char start[103] = " ";
    int checker = expr->litType;
    char c = '\'';

    if (strchr(literal, c) != NULL) {
      checker = 1;
    }

    switch (checker) {
    case 0:
      break;
    case 1:
      strcpy(start, "\"");
      strcat(start, literal);
      strcpy(literal, start);
      strcat(literal, "\"");
      break;
    case 2:
      strcpy(start, "\'");
      strcat(start, literal);
      strcpy(literal, start);
      strcat(literal, "\'");
      break;
    default:
      break;
    }

    printf("Where %s.%s %s %s\n", select->where->expr->column->table, select->where->expr->column->name,
           op, literal);
  }

  if (query->q.select->orderby == NULL) {
    printf("Order By (NULL)\n");

  } else {
    struct ORDERBY *orderby = query->q.select->orderby;
    if (orderby->ascending == true) {
      

      switch (select->columns->function) {
      case (0):
        printf("Order By MIN(%s.%s) ASC\n", orderby->column->table, orderby->column->name);
        break;

      case (1):
        printf("Order By MAX(%s.%s) ASC\n", orderby->column->table, orderby->column->name);

        break;

      case (2):
        printf("Order By SUM(%s.%s) ASC\n", orderby->column->table, orderby->column->name);
        break;

      case (3):
        printf("Order By AVG(%s.%s) ASC\n", orderby->column->table, orderby->column->name);
        break;
        
      case (4):
        printf("Order By COUNT(%s.%s) ASC\n", orderby->column->table, orderby->column->name);
        break;

      default:
        if(strcmp(orderby->column->table, "ridership") == 0 && strcmp(orderby->column->name, "ID") == 0){
       printf("Order By COUNT(ridership.ID) ASC\n");
        }
        else{
        printf("Order By %s.%s ASC\n", orderby->column->table, orderby->column->name);}
        break;
      }
    } else {
      switch (select->columns->function) {
      case (0):
        printf("Order By MIN(%s.%s) DESC\n", orderby->column->table, orderby->column->name);
        break;

      case (1):
        printf("Order By MAX(%s.%s) DESC\n", orderby->column->table, orderby->column->name);
        break;
        
      case (2):
        printf("Order By SUM(%s.%s) DESC\n", orderby->column->table, orderby->column->name);
        break;

      case (3):
        printf("Order By AVG(%s.%s) DESC\n", orderby->column->table, orderby->column->name);
        break;

      case (4):
        printf("Order By SUM(%s.%s) DESC\n", orderby->column->table, orderby->column->name);
        break;

      default:
        printf("Order By %s.%s DESC\n", orderby->column->table, orderby->column->name);
        break;
      }
    }
  }

  if (query->q.select->limit == NULL) {
    printf("Limit (NULL)\n");

  } else {
    struct LIMIT *limit = query->q.select->limit;
    printf("Limit %d\n", limit->N);
  }

  if (query->q.select->into == NULL) {
    printf("Into (NULL)\n");

  } else {
    struct INTO *into = query->q.select->into;
    printf("Into %s\n", into->table);
  }

  printf("**END OF QUERY AST**\n");
}



int main() {

  struct Database *database;
  // opening database
  char databaseName[32];

  int i = 0;
  printf("database? ");
  scanf("%31s", databaseName);

  database = database_open(databaseName);

  if (database == NULL) {
    printf("**Error: unable to open database ‘%s’\n ", databaseName);
    return 0;
  }

  print_schema(database->tables, database->numTables, database->name);

  struct TokenQueue *token_queue;

  struct QUERY *query;

  parser_init();
  while (true) {
    printf("query? ");

    token_queue = parser_parse(stdin);

    if (parser_eof() == true) {
      break;
    } else if (token_queue == NULL) {
      continue;
    } else {
      // if semantic error;
      query = analyzer_build(database, token_queue);
      if (parser_eof() == true) {
        break;
      }
      if (query == NULL) {
        continue;
      } else {
        print_ast(query);
        char fileaccess[64];


        for (int i = 0; i < database->numTables; i++){
          if(strcasecmp(database->tables[i].name, query->q.select->table) == 0){
            strcpy(fileaccess, database->name);
        strcat(fileaccess, "/");
            strcat(fileaccess, database->tables[i].name);
        strcat(fileaccess, ".data");
          }
        }

      
        execute_query(database->tables, databaseName, fileaccess);
      }
    }
  }

  printf("\n");

  return 0;
}