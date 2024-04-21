/*main.c*/

//
// Unit tests for SimpleSQL execute_query component.
// These tests are written using the Google Test 
// framework.
//
// Mark Chauhan
// Northwestern University
// CS 211, Winter 2023
//
// Initial version: Prof. Joe Hummel
//

#include <assert.h>  // assert
#include <ctype.h>   // isdigit
#include <math.h>    // fabs
#include <stdbool.h> // true, false
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcpy, strcat

#include "analyzer.h"
#include "ast.h"
#include "database.h"
#include "execute.h"
#include "parser.h"
#include "resultset.h"
#include "scanner.h"
#include "tokenqueue.h"
#include "util.h"

#include "gtest/gtest.h"

//
// Test case: Select * From Movies2 limit 3;
//
// NOTE: this was presented in class on Tues, Jan 31.
// This test is *NOT* complete and considered an
// example of how to structure a test, but it needs
// to be extended with more checking:
//    - correct # of columns, in correct order / types?
//    - correct # of rows?
//    - is the data in each row correct?
//

TEST(execute_query, checker2){
  FILE *output = fopen("input.txt", "w");
  fprintf(output, "Select ID, Title From Movies2 where Revenue < 100000000;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("MovieLens2");
  parser_init();

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL);

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(query != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);

  

  //check the data that should be in row 2
  ASSERT_TRUE(resultset_getInt(rs, 4, 1) == 750);
  //ASSERT_TRUE(strcasecmp(resultset_getString(rs, 4, 2), "Murder She Said") == 0);

  //2 columns means that the where has been found, since the other columns have been deleted 
  ASSERT_TRUE(rs->numCols == 2);
  ASSERT_TRUE(rs->numRows == 7);
  
  ASSERT_TRUE(query->q.select->where != NULL);
  ASSERT_TRUE(query->q.select->where->expr->Operator == 0);
  ASSERT_TRUE(query->q.select->where->expr->litType == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->value, "100000000") == 0);

  struct RSColumn* column1 = rs->columns; 
  ASSERT_TRUE(column1 != NULL);
  ASSERT_TRUE(column1->coltype == 1);
  ASSERT_TRUE(strcasecmp(column1->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column1->colName, "ID") == 0);
  ASSERT_TRUE(column1->function == -1);

   
  
  struct RSColumn* column2 = column1->next; 
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->coltype == 3);
  ASSERT_TRUE(strcasecmp(column2->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column2->colName, "Title") == 0);
  ASSERT_TRUE(column2->function == -1);
  
  
  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 == NULL);
  
}

TEST(execute_query, checker3){
  
  FILE *output = fopen("input.txt", "w");
  fprintf(output,  "select min(ID), max(date), count(daytype), avg(riders) from Ridership;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("CTA"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);

  ASSERT_TRUE(rs->numCols == 4);
  ASSERT_TRUE(rs->numRows == 1);
  
  ASSERT_TRUE(resultset_getInt(rs, 1, 1) == 40010);
  ASSERT_TRUE(strcasecmp(resultset_getString(rs, 1, 2), "2018-11-30")  == 0);
  ASSERT_TRUE(resultset_getInt(rs, 1, 3) == 10239);
  ASSERT_TRUE(resultset_getReal(rs, 1, 4) - 3188.581600 < 0.0000001);

  struct RSColumn* column1 = rs->columns;
  ASSERT_TRUE(strcasecmp(column1->tableName, "Ridership") == 0);
  ASSERT_TRUE(column1->function == 0);
  ASSERT_TRUE(strcasecmp(column1->colName, "ID") == 0);
  ASSERT_TRUE(column1 != NULL);
  ASSERT_TRUE(column1->coltype == 1);

  struct RSColumn* column2 = column1->next; 
  ASSERT_TRUE(strcasecmp(column2->tableName, "Ridership") == 0);
  ASSERT_TRUE(strcasecmp(column2->colName, "Date") == 0);
  ASSERT_TRUE(column2->function == 1);
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->coltype == 3);
  
  struct RSColumn* column3 = column2->next; 
  ASSERT_TRUE(strcasecmp(column3->tableName, "Ridership") == 0);
  ASSERT_TRUE(strcasecmp(column3->colName, "Daytype") == 0);
  ASSERT_TRUE(column3->function == 4);
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->coltype == 1);
  
  struct RSColumn* column4 = column3->next; 
  ASSERT_TRUE(strcasecmp(column4->tableName, "Ridership") == 0);
  ASSERT_TRUE(strcasecmp(column4->colName, "Riders") == 0);
  ASSERT_TRUE(column4->function == 3);
  ASSERT_TRUE(column4 != NULL);
  ASSERT_TRUE(column4->coltype == 2);
  ASSERT_TRUE(column4->next == NULL);
  
  
}

TEST(execute_query, checker4){
  
  FILE *output = fopen("input.txt", "w");
  fprintf(output,  "select sum(Riders) from Ridership where daytype <> \'W\';\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("CTA"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);

  ASSERT_TRUE(query->q.select->where != NULL);
  ASSERT_TRUE(query->q.select->where->expr->Operator == 5);
  ASSERT_TRUE(query->q.select->where->expr->litType == 2);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->column->name, "daytype") == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->value, "W") == 0);
  ASSERT_TRUE(rs->numCols == 1);
  ASSERT_TRUE(rs->numRows == 1);

  ASSERT_TRUE(resultset_getInt(rs, 1, 1) == 5699705);

  ASSERT_TRUE(rs->columns != NULL);
  ASSERT_TRUE(rs->columns->coltype == 1);
  ASSERT_TRUE(strcasecmp(rs->columns->tableName, "Ridership") == 0);
  ASSERT_TRUE(strcasecmp(rs->columns->colName, "Riders") == 0);
  ASSERT_TRUE(rs->columns->function == 2);
  ASSERT_TRUE(rs->columns->next == NULL);
  
}

TEST(execute_query, checker5){
  
  FILE *output = fopen("input.txt", "w");
  fprintf(output,  "select ID, sum(Revenue) from Movies where Title <= 'Sleepless in Seattle';\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("MovieLens"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);

  ASSERT_TRUE(rs->numCols == 2);
  ASSERT_TRUE(rs->numRows == 1);

  ASSERT_TRUE(query->q.select->where != NULL);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->column->name, "Title") == 0);
  ASSERT_TRUE(query->q.select->where->expr->Operator == 1);
  ASSERT_TRUE(query->q.select->where->expr->litType == 2);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->value, "Sleepless in Seattle") == 0);

  ASSERT_TRUE(rs->columns != NULL);
  ASSERT_TRUE(rs->columns->coltype == 1);
  ASSERT_TRUE(strcasecmp(rs->columns->tableName , "Movies") == 0);
  ASSERT_TRUE(strcasecmp(rs->columns->colName , "ID") == 0);
  ASSERT_TRUE(rs->columns->function == -1);
  
  ASSERT_TRUE(rs->columns->next != NULL);
  ASSERT_TRUE(rs->columns->next->coltype == 2);
  ASSERT_TRUE(strcasecmp(rs->columns->next->tableName , "Movies") == 0);
  ASSERT_TRUE(strcasecmp(rs->columns->next->colName , "Revenue") == 0);
  ASSERT_TRUE(rs->columns->next->function == 2);
  
  ASSERT_TRUE(rs->columns->next->next == NULL);
  
}

TEST(execute_query, checker6){
  
   FILE *output = fopen("input.txt", "w");
  
  fprintf(output,  "select Riders, Date from Ridership where ID = 40400 limit 5;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("CTA"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);

  ASSERT_TRUE(rs->numCols == 2);
  ASSERT_TRUE(rs->numRows == 5);

  ASSERT_TRUE(rs->columns != NULL);

  ASSERT_TRUE(query->queryType == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->columns->table, "Ridership") == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->columns->name, "Riders") == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->columns->next->table, "Ridership") == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->columns->next->name, "Date") == 0);
  ASSERT_TRUE(query->q.select->where->expr->Operator == 4);
  ASSERT_TRUE(query->q.select->where->expr->litType == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->column->name, "ID") == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->value, "40400" ) == 0);
  ASSERT_TRUE(query->q.select->columns->next->next == NULL);

  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->column->table, "Ridership") == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->column->name, "ID") == 0);
  
  
  ASSERT_TRUE(rs->columns->coltype == 1);
  ASSERT_TRUE(strcasecmp(rs->columns->tableName , "Ridership") == 0);
  ASSERT_TRUE(strcasecmp(rs->columns->colName , "Riders") == 0);
  ASSERT_TRUE(rs->columns->function == -1);
  
  ASSERT_TRUE(rs->columns->next != NULL);
  ASSERT_TRUE(rs->columns->next->coltype == 3);
  ASSERT_TRUE(strcasecmp(rs->columns->next->tableName , "Ridership") == 0);
  ASSERT_TRUE(strcasecmp(rs->columns->next->colName , "Date") == 0);
  ASSERT_TRUE(rs->columns->next->function == -1);
  
  ASSERT_TRUE(rs->columns->next->next == NULL);
  
}

TEST(execute_query, checker7){
  
   FILE *output = fopen("input.txt", "w");
  
  fprintf(output,  "select * from Movies2 limit 10;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("MovieLens2"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);

  ASSERT_TRUE(rs->numCols == 4);
  ASSERT_TRUE(rs->numRows == 9);

  ASSERT_TRUE(rs->columns != NULL);

  ASSERT_TRUE(query->queryType == 0);
 


  
  
}

TEST(execute_query, checker8){
  FILE *output = fopen("input.txt", "w");
  
  fprintf(output,  "select avg(rating) from Ratings;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("MovieLens"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);

  ASSERT_TRUE(rs->numCols == 1);
  ASSERT_TRUE(rs->numRows == 1);

  ASSERT_TRUE(rs->columns != NULL);


  ASSERT_TRUE(query->q.select->columns->function == 3);
  ASSERT_TRUE(rs->columns->function == 3);
  ASSERT_TRUE(rs->columns->coltype == 2);
  
  
}

TEST(execute_query, checker9){
  FILE *output = fopen("input.txt", "w");
  
  fprintf(output,  "select * from Movies2 where ID < 1000;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("MovieLens2"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);

  //resultset_print(rs);
  ASSERT_TRUE(rs->numCols == 4);
  ASSERT_TRUE(rs->numRows == 5);

  ASSERT_TRUE(query->q.select->where->expr->Operator == 0);
  ASSERT_TRUE(query->q.select->where->expr->litType == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->value, "1000") == 0);
  ASSERT_TRUE(rs->columns != NULL);

  ASSERT_TRUE(resultset_getInt(rs, 1, 1) == 912);
  ASSERT_TRUE(strcasecmp(resultset_getString(rs, 1, 2), "The Thomas Crown Affair") == 0);
  
  ASSERT_TRUE(resultset_getInt(rs, 2, 1) == 111);
  ASSERT_TRUE(strcasecmp(resultset_getString(rs, 2, 2), "Scarface") == 0);
  ASSERT_TRUE(resultset_getInt(rs, 3, 1) == 260);
  ASSERT_TRUE(resultset_getInt(rs, 4, 1) == 750);
  ASSERT_TRUE(resultset_getInt(rs, 5, 1) == 293);
  
  struct RSColumn* column1 =rs->columns;     
  ASSERT_TRUE(column1 != NULL);
  ASSERT_TRUE(column1->coltype == 1);
  ASSERT_TRUE(strcasecmp(column1->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column1->colName, "ID") == 0);
  ASSERT_TRUE(column1->function == -1);
    
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->coltype == 3);
  ASSERT_TRUE(strcasecmp(column2->tableName, "Movies2") == 0);\
  ASSERT_TRUE(strcasecmp(column2->colName, "Title") == 0);
  ASSERT_TRUE(column2->function == -1);
  
  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->coltype == 1);
  ASSERT_TRUE(strcasecmp(column3->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column3->colName, "Year") == 0);
  ASSERT_TRUE(column3->function == -1);

  struct RSColumn* column4 = column3->next;
  ASSERT_TRUE(column4 != NULL);
  ASSERT_TRUE(column4->coltype == 2);
  ASSERT_TRUE(strcasecmp(column4->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column4->colName, "Revenue") == 0);
  ASSERT_TRUE(column4->function == -1);

  ASSERT_TRUE(column4->next == NULL);
}

TEST(execute_query, checker10){
  FILE *output = fopen("input.txt", "w");
  
  fprintf(output,  "select * from Movies2 where ID > 300;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("MovieLens2"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);

  ASSERT_TRUE(rs->numCols == 4);
  ASSERT_TRUE(rs->numRows == 6);

  ASSERT_TRUE(query->q.select->where->expr->Operator == 2);
  ASSERT_TRUE(query->q.select->where->expr->litType == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->value, "300") == 0);
  ASSERT_TRUE(rs->columns != NULL);

  ASSERT_TRUE(resultset_getInt(rs, 2, 1) == 750);
  ASSERT_TRUE(strcmp(resultset_getString(rs, 2, 2), "Murder She Said") == 0);
  ASSERT_TRUE(resultset_getInt(rs, 2, 3) == 1961);
  
  ASSERT_TRUE(rs->columns != NULL);

//Checking if each column should have something
//Checking the Column Type of Each Column
  struct RSColumn* column1 =rs->columns;     
  ASSERT_TRUE(column1 != NULL);
  ASSERT_TRUE(column1->coltype == 1);
  ASSERT_TRUE(strcasecmp(column1->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column1->colName, "ID") == 0);
  ASSERT_TRUE(column1->function == -1);
    
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->coltype == 3);
  ASSERT_TRUE(strcasecmp(column2->tableName, "Movies2") == 0);\
  ASSERT_TRUE(strcasecmp(column2->colName, "Title") == 0);
  ASSERT_TRUE(column2->function == -1);
  
  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->coltype == 1);
  ASSERT_TRUE(strcasecmp(column3->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column3->colName, "Year") == 0);
  ASSERT_TRUE(column3->function == -1);
  
  struct RSColumn* column4 = column3->next;
  ASSERT_TRUE(column4 != NULL);
  ASSERT_TRUE(column4->next == NULL);
  ASSERT_TRUE(column4->coltype == 2); 
  ASSERT_TRUE(strcasecmp(column4->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column4->colName, "Revenue") == 0);
  ASSERT_TRUE(column4->function == -1);
  // column 4 should be the last column
  ASSERT_TRUE(column4->next == NULL);
}

TEST(execute_query, checker11){
  FILE *output = fopen("input.txt", "w");
  
  fprintf(output,  "select Revenue, ID, Title from Movies2 where ID <= 912;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("MovieLens2"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);
  //resultset_print(rs);
  ASSERT_TRUE(rs->numCols == 3);
  ASSERT_TRUE(rs->numRows == 5);

  ASSERT_TRUE(query->queryType == 0);
  ASSERT_TRUE(query->q.select->where->expr->Operator == 1);
  ASSERT_TRUE(query->q.select->where->expr->litType == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->value, "912") == 0);
  ASSERT_TRUE(rs->columns != NULL);

//  ASSERT_TRUE(strcmp(resultset_getString(rs, 2, 4), "License to Wed") == 0);
  //ASSERT_TRUE(fabs(resultset_getReal(rs, 2, 2) - 69307224.0) < 0.00001);
  
  ASSERT_TRUE(rs->columns != NULL);
  //resultset_print(rs);

  ASSERT_TRUE(resultset_getInt(rs, 1,2) == 912);
  ASSERT_TRUE(strcasecmp(resultset_getString(rs, 1, 3), "The Thomas Crown Affair") == 0);
  
  ASSERT_TRUE(resultset_getInt(rs, 2,2) == 111);
  ASSERT_TRUE(strcasecmp(resultset_getString(rs, 2, 3), "Scarface") == 0);

  ASSERT_TRUE(resultset_getInt(rs, 3,2) == 260);
  ASSERT_TRUE(strcasecmp(resultset_getString(rs, 3, 3), "The 39 Steps") == 0);
//Checking if each column should have something
//Checking the Column Type of Each Column
  struct RSColumn* column1 =rs->columns;     
  ASSERT_TRUE(column1 != NULL);
  ASSERT_TRUE(column1->coltype == 2);
  ASSERT_TRUE(strcasecmp(column1->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column1->colName, "Revenue") == 0);
  ASSERT_TRUE(column1->function == -1);
    
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->coltype == 1);
  ASSERT_TRUE(strcasecmp(column2->tableName, "Movies2") == 0);\
  ASSERT_TRUE(strcasecmp(column2->colName, "ID") == 0);
  ASSERT_TRUE(column2->function == -1);

  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->coltype == 3);
  ASSERT_TRUE(strcasecmp(column3->tableName, "Movies2") == 0);\
  ASSERT_TRUE(strcasecmp(column3->colName, "Title") == 0);
  ASSERT_TRUE(column3->function == -1);
  
  // column 4 should be the last column
  ASSERT_TRUE(column3->next == NULL);
}

TEST(execute_query, checker12){
  FILE *output = fopen("input.txt", "w");
  
  fprintf(output,  "select Revenue, ID, Title from Movies2 where ID < 912;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  struct Database *db = database_open("MovieLens2"); 
  parser_init(); 

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL); 

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(tq != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);
  //resultset_print(rs);
  ASSERT_TRUE(rs->numCols == 3);
  ASSERT_TRUE(rs->numRows == 4);

  ASSERT_TRUE(query->queryType == 0);
  ASSERT_TRUE(query->q.select->where->expr->Operator == 0);
  ASSERT_TRUE(query->q.select->where->expr->litType == 0);
  ASSERT_TRUE(strcasecmp(query->q.select->where->expr->value, "912") == 0);
  ASSERT_TRUE(rs->columns != NULL);

//  ASSERT_TRUE(strcmp(resultset_getString(rs, 2, 4), "License to Wed") == 0);
  //ASSERT_TRUE(fabs(resultset_getReal(rs, 2, 2) - 69307224.0) < 0.00001);
  
  ASSERT_TRUE(rs->columns != NULL);

//Checking if each column should have something
//Checking the Column Type of Each Column
  struct RSColumn* column1 =rs->columns;     
  ASSERT_TRUE(column1 != NULL);
  ASSERT_TRUE(column1->coltype == 2);
  ASSERT_TRUE(strcasecmp(column1->tableName, "Movies2") == 0);
  ASSERT_TRUE(strcasecmp(column1->colName, "Revenue") == 0);
  ASSERT_TRUE(column1->function == -1);
    
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->coltype == 1);
  ASSERT_TRUE(strcasecmp(column2->tableName, "Movies2") == 0);\
  ASSERT_TRUE(strcasecmp(column2->colName, "ID") == 0);
  ASSERT_TRUE(column2->function == -1);

  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->coltype == 3);
  ASSERT_TRUE(strcasecmp(column3->tableName, "Movies2") == 0);\
  ASSERT_TRUE(strcasecmp(column3->colName, "Title") == 0);
  ASSERT_TRUE(column3->function == -1);
  
  // column 4 should be the last column
  ASSERT_TRUE(column3->next == NULL);
}



//
// TODO: add more tests!
//




//
// main program: do not change this, google test will find all your
// tests and run them.  So leave the code below as is!
//
int main() {
  ::testing::InitGoogleTest();

  //
  // run all the tests, returns 0 if they
  // all pass and 1 if any fail:
  //
  int result = RUN_ALL_TESTS();

  return result; // 0 => all passed
}