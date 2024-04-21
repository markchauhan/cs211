//execute.c*/

//
// Project: Execution of queries for SimpleSQL
//
// Mark Chauhan
// Northwestern University
// CS 211, Winter 2023
//

#include <assert.h>
#include <ctype.h>
#include <stdbool.h> // true, false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// #include any other system <.h> files?
//

#include "execute.h"
#include "resultset.h"
//
// #include any other of our ".h" files?
//

void execute_query(struct Database *db, struct QUERY *query) {
  if (db == NULL)
    panic("db is NULL (execute)");
  if (query == NULL)
    panic("query is NULL (execute)");

  if (query->queryType != SELECT_QUERY) {
    printf("**INTERNAL ERROR: execute() only supports SELECT queries.\n");
    return;
  }

  struct SELECT *select = query->q.select; // alias for less typing:

  //
  // the query has been analyzed and so we know it's correct: the
  // database exists, the table(s) exist, the column(s) exist, etc.
  //

  //
  // (1) we need a pointer to the table meta data, so find it:
  //
  struct TableMeta *tablemeta = NULL;

  for (int t = 0; t < db->numTables; t++) {
    if (icmpStrings(db->tables[t].name, select->table) == 0) // found it:
    {
      tablemeta = &db->tables[t];
      //      rs->columns = &db->tables[t];
      // printf("%s\n", db->tables[t]);
      //      printf("%s\n", rs->columns->tableName);
      break;
    }
  }

  struct ResultSet *rs = resultset_create();

  for (int q = 0; q < tablemeta->numColumns; q++) {
    resultset_insertColumn(rs, q + 1, tablemeta->name,
                           tablemeta->columns[q].name, NO_FUNCTION,
                           tablemeta->columns[q].colType);
  }

  assert(tablemeta != NULL);

  //
  // (2) open the table's data file
  //
  // the table exists within a sub-directory under the executable
  // where the directory has the same name as the database, and with
  // a "TABLE-NAME.data" filename within that sub-directory:
  //
  char path[(2 * DATABASE_MAX_ID_LENGTH) + 10];

  strcpy(path, db->name); // name/name.data
  strcat(path, "/");
  strcat(path, tablemeta->name);
  strcat(path, ".data");

  FILE *datafile = fopen(path, "r");
  if (datafile == NULL) // unable to open:
  {
    printf("**INTERNAL ERROR: table's data file '%s' not found.\n", path);
    panic("execution halted");
  }

  //
  // (3) allocate a buffer for input, and start reading the data:
  //
  int dataBufferSize =
      tablemeta->recordSize + 3; // ends with $\n + null terminator
  char *dataBuffer = (char *)malloc(sizeof(char) * dataBufferSize);
  if (dataBuffer == NULL)
    panic("out of memory");

  // for (int record = 0; record < 5; record++)
  //  {
  //    fgets(dataBuffer, dataBufferSize, datafile);

  //    if (feof(datafile)) // end of the data file, we're done
  //      break;

  //    printf("%s", dataBuffer);
  //  }

 char* cp;
 int numr = 0;
 int numc = 0;

 struct RSColumn *temp; 
  


  while (true) {
  
    fgets(dataBuffer, dataBufferSize, datafile);

    if (feof(datafile)) { // end of the data file, we're done
      break;
    }
    
      resultset_addRow(rs);
      numr++;
      
  //  printf("%s", dataBuffer);
    
    

    cp = dataBuffer;

    //loops until reaches End and inserts '\0' at quotations and spaces
    while(*cp != '$'){
      
      if (*cp == ' ' ){
        *cp = '\0';
        cp++;
      }
      else{
        if(*cp == '\''){
          
        
          cp++;
          while(*cp != '\''){
            cp++;
          }
          
          if(*cp == '\''){
            *cp = '\0';
            cp++;
          }
          
      
        }
        else if(*cp == '\"'){
          
          cp++;
        
          while(*cp != '\"'){
            cp++;
          }

          if(*cp == '\"'){
            *cp = '\0';
            cp++;
          }
          
  
        }
        cp++;
      }
    }
    
    cp = dataBuffer;
    numc = 0;
    
    //takes pointer cp and inserts values with broken apart strings
    //each time cp++ is called skips forward to next string
    while(numc < tablemeta->numColumns){
      
      if(*cp == '\0' || numc == 0){
      if(*cp == '\0'){
          cp++;
        }

        
      if(tablemeta->columns[numc].colType == COL_TYPE_INT){
        resultset_putInt(rs, numr, numc + 1, atoi(cp));
        numc++;
        cp++;
      }
      else if(tablemeta->columns[numc].colType == COL_TYPE_REAL){
        resultset_putReal(rs, numr, numc + 1, atof(cp));
        numc++;
        cp++;
      }
      else if(tablemeta->columns[numc].colType == COL_TYPE_STRING){
        for(int r = 0;  r < numc; r++){
        cp++;}
        
        
        resultset_putString(rs, numr, numc + 1, cp);
        numc++;
        
      }} else{
        cp++;
      }
    }
      
    }
  
  //closes file and frees area
  fclose(datafile);
  free(dataBuffer);



  //Check Where
  
 
  int numc_where;
  if(query->q.select->where != NULL){
    //goes through all of the table columns to see which table(column) matches the query's wants
  for(int j = 0; j < tablemeta->numColumns; j++){
    
    if(strcasecmp(tablemeta->columns[j].name, query->q.select->where->expr->column->name) == 0){ 

      numc_where = j + 1; 
      
//compares if string and delete unneeded info

      if(query->q.select->where->expr->litType == 2 || tablemeta->columns[j].colType == 3){
  
      for(int k = numr; k > 0; k--){
        switch(query->q.select->where->expr->operator) {
          case 0:
            if(strcasecmp(query->q.select->where->expr->value, resultset_getString(rs, k, numc_where)) <= 0){
              resultset_deleteRow(rs, k); 
            }
            break;
          case 1:
            if(strcasecmp(query->q.select->where->expr->value, resultset_getString(rs, k, numc_where)) < 0){
              resultset_deleteRow(rs, k); 
            }
            
            break;
          case 2:
            if(strcasecmp(query->q.select->where->expr->value, resultset_getString(rs, k, numc_where)) >= 0){
              resultset_deleteRow(rs, k); 
            }
            
            break;
          case 3:
            if(strcasecmp(query->q.select->where->expr->value, resultset_getString(rs, k, numc_where)) > 0){
              resultset_deleteRow(rs, k);
              }
            break;
          case 4:
            if(strcasecmp(query->q.select->where->expr->value, resultset_getString(rs, k, numc_where)) != 0){
              resultset_deleteRow(rs, k); 
            }
            break;
          case 5:
            if(strcasecmp(query->q.select->where->expr->value, resultset_getString(rs, k, numc_where)) == 0){
              resultset_deleteRow(rs, k); 
            }
            break;
            
          default:
            break;
          }
        
        
      }
      }//compars if real and dete unneeded info
      else if(query->q.select->where->expr->litType == 1 || tablemeta->columns[j].colType == 2){
        for(int l = numr; l > 0; l--){
        switch(query->q.select->where->expr->operator) {
          case 0:
              if(resultset_getReal(rs, l, numc_where) >= atof(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, l); 
        
              }
            break;
          case 1:
            if(resultset_getReal(rs, l, numc_where) > atof(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, l); 
    
              }
            break;
          case 2:
            if(resultset_getReal(rs, l, numc_where) <= atof(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, l); 

              }
            break;
          case 3:
            if(resultset_getReal(rs, l, numc_where) < atof(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, l); 

              }
            
            break;
          case 4:
            if(resultset_getReal(rs, l, numc_where) != atof(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, l); 
         
              }
            break;
          case 5:
            if(resultset_getReal(rs, l, numc_where) == atof(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, l); 
    
              }
            break;
          default:
            break;
            
          }
      }
        }//compares if int and deletes unneeded info
      else{
        for(int m = numr; m > 0; m--){

          
        switch(query->q.select->where->expr->operator) {
          case 0:
              if(resultset_getInt(rs, m, numc_where) >= atoi(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, m); 
        
              }
            break;
          case 1:
            if(resultset_getInt(rs, m, numc_where) > atoi(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, m); 
        
              }
            break;
          case 2:
            if(resultset_getInt(rs, m, numc_where) <= atoi(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, m); 
    
              }
            break;
          case 3:
            if(resultset_getInt(rs, m, numc_where ) < atoi(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, m); 
            
              }
            
            break;
          case 4:
            if(resultset_getInt(rs, m, numc_where) != atoi(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, m); 
              
              }
            break;
          case 5:
            if(resultset_getInt(rs, m, numc_where) == atoi(query->q.select->where->expr->value)){
                resultset_deleteRow(rs, m); 
              
              }
            break;
          default:
            break;
            
          }
        
        
      }
      }
    }

  }

  }
  

  


//Looks through all columns of tablemeta and compares to the linked list of columns


  
  


for(int w = rs->numCols; w >= 1; w--){
  struct COLUMN* checker = select->columns;
  bool flag = true;
  while (true) {
    if(strcasecmp(checker->name, tablemeta->columns[w-1].name) == 0){
      break;
      }else {
      
    }

    if (checker->next != NULL) {
      checker = checker->next;
    } else {
      if (flag) {
        resultset_deleteColumn(rs, w);
      }
      break;
    }
    }
  
}
  int pos = 1;
  struct COLUMN* astCol = query->q.select->columns;

  for (int _ = 1; _ <= rs->numCols; _++) {

    resultset_moveColumn(rs, resultset_findColumn(rs, 1, astCol->table, astCol->name), pos);

    if (astCol->next != NULL) {
      astCol = astCol->next;
      pos++;
    } else {
      break;
    }
  }
  
   
  
  struct COLUMN *find = query->q.select->columns;
  int function_number = 1;
          //looks if there is applicable functions and then applies  
  while(true){
    
    if(find == NULL){
      break;
    }
      switch(find->function){
        case 0:
          resultset_applyFunction(rs,0, function_number);

          break;
        case 1:
          resultset_applyFunction(rs,1, function_number);

          break;
        case 2:
          resultset_applyFunction(rs,2,function_number);

          break;
        case 3:
          resultset_applyFunction(rs,find->function,function_number);

          break;
        case 4:
          resultset_applyFunction(rs,4,function_number);
          break;
        default:
          
        break;}

    function_number++;
    find = find->next;
    
    }
//asks if there is a limit
  //if so then delete from the highest row to the limit
    if(query->q.select->limit != NULL){
    for(int a = rs->numRows; a > query->q.select->limit->N; a--){
      resultset_deleteRow(rs, a);
    }
      }

  
  resultset_print(rs);
  resultset_destroy(rs);
  //
  // done!
  //
}
// implementation of function(s), both private and public
//