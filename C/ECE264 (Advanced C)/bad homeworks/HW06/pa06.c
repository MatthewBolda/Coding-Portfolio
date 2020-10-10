#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "pa06.h"
#define MAX_LEN 80

#ifndef TEST_CONSRUCT
/* 
	Construct SelectedField.
	1. This function returns a pointer to SelectedField object.
	2. Allocate memory for SelectedField object.
	3. Initialize all the fields in this object to "false".
	4. return NULL if you fail to allocate memory for SelectedField object.
*/
SelectedField * ConstructField() {

	SelectedField * fields = malloc(sizeof(SelectedField));
	// maybe SelectedField * fields = malloc(SelectedField); ??????????????????
	if(fields == NULL){
		printf("Fail to allocate memory\n");
		return NULL;
	}
	//initialized all as false	
	fields -> id = false;
	fields -> name = false;
	fields -> major = false;
	fields -> year = false;
	fields -> enrollment = false;
	fields -> age = false;

	return fields;


}
#endif

#ifndef TEST_PARSE
/* 
	Parse the query and store the information into ParseResult object.
	1. This function returns a pointer to ParseResult object.
	2. The ParseResult object has four fields.
		1. Pointer to SelectedField object. The SelectedField  object stores the informations of which fields are selected in the query. For example, if field id is selected, you should update that field to "true".
		2. Number of condition. 
		3. Array of pointers to Condition object. 
		4. What logic does this query specify. It should be eigther AND or OR.
	3. In this function, you should parse the qeury and store the information to ParseResult object. Similar to `Connect()` function in HW04, you should allocate memory for the required objects and then update the fields in the objects. 
*/
ParseResult * ParseQuery(int num, char ** query) {

int i = 0;
int lognum = 1;
int condnum =0;

ParseResult * parsedQ = malloc(sizeof(ParseResult));

parsedQ -> field = ConstructField();

// converts falses to trues
while(strcmp(query[i], "WHERE") ){
	
	if(strcmp(query[i],"id") == 0){
		parsedQ -> field -> id = true;
        
	}
	if(strcmp(query[i],"name") == 0){
		 parsedQ -> field -> name = true;
        
	}
	if(strcmp(query[i],"major") == 0){
		 parsedQ -> field -> major = true;
        
	}
	if(strcmp(query[i],"year") == 0){
		 parsedQ -> field -> year = true;
        
	}
	if(strcmp(query[i],"enrollment") == 0){
		 parsedQ -> field -> enrollment = true;
        
	}
	if(strcmp(query[i],"age") == 0){	
		 parsedQ -> field -> age = true;		
	
	}
i++;
}

// the logic bit
// counting how many AND and OR conditions there are
	while (i < num){
		if(strcmp(query[i], "AND") == 0){
			strcpy(parsedQ -> logic, "AND");
			lognum ++;
		}	
		if(strcmp(query[i], "OR") ==0){
			strcpy(parsedQ -> logic, "OR");
			lognum++;
		}
	i++;
	}
	
	// tell the parsedQ how many condition numbers there are
	parsedQ -> condition_num = lognum;
	//allocate memory as much as needed for the logical amount
	parsedQ -> conditions = malloc(sizeof(Condition)*lognum);
	//allocate memory for each individual condition
	for(int j = 0; j < lognum; j++){
		parsedQ -> conditions[j] = malloc(sizeof(Condition));
		}
	//initializing counter
	int x=0;
	// goes until the WHERE then goes one more to start looking for next arguments
	while(strcmp(query[x], "WHERE") != 0){
		x = x+1;
		}

	while(x < num){
		printf("%s", query[x]);
		strcpy(parsedQ -> conditions[condnum] -> field, query[x]);
		x = x+1;
		strcpy(parsedQ -> conditions[condnum] -> optr, query[x]);	
		x = x+1;
		strcpy(parsedQ -> conditions[condnum] -> val, query[x]);
		x = x+1;
		if(strcmp(query[x], "AND") == 0 || strcmp(query[x], "OR") == 0){
			x++;
		}
		condnum = condnum + 1;
	}	
}
#endif


#ifndef TEST_COMPARE
/*
	This is a helper function we provide in this assignment.
	It checks whether a stduent meets the condition.
	You can choose not to use this function.
	Do not modify this function. If you change it, you will get zero.
*/
bool Compare(Student * stu, Condition * condition) {
	if (strcmp(condition -> field, "id") == 0) {
		int id_val = atoi(condition -> val);
		if (strcmp(condition -> optr, "<") == 0) {
			return (stu -> id < id_val);
		} else if (strcmp(condition -> optr, ">") == 0) {
			return (stu -> id > id_val);
		} else if (strcmp(condition -> optr, "=") == 0) {
			return stu -> id == id_val;
		} else if (strcmp(condition -> optr, ">=") == 0) {
			return (stu -> id >= id_val);
		} else if (strcmp(condition -> optr, "<=") == 0) {
			return (stu -> id <= id_val);
		}
	} else if (strcmp(condition -> field, "age") == 0) {
		int age_val = atoi(condition -> val);
		if (strcmp(condition -> optr, "<") == 0) {
			return (stu -> age < age_val);
		} else if (strcmp(condition -> optr, ">") == 0) {
			return (stu -> age > age_val);
		} else if (strcmp(condition -> optr, "=") == 0) {
			return (stu -> age == age_val);
		} else if (strcmp(condition -> optr, ">=") == 0) {
			return (stu -> age >= age_val);
		} else if (strcmp(condition -> optr, "<=") == 0) {
			return (stu -> age <= age_val);
		}
	} else {
		if (strcmp(condition -> field, "name") == 0	) {
			if (strcmp(stu -> name, condition -> val) == 0) {
				return true;	
			}
			return false;	
		} else if (strcmp(condition -> field, "major") == 0 ) {
			if (strcmp(stu -> major, condition -> val) == 0){
				return true;	
			}
			return false;	
		} else if (strcmp(condition -> field, "year") == 0 ) {
			if (strcmp(stu -> year, condition -> val) == 0){
				return true;	
			}
			return false;	
		} else if (strcmp(condition -> field, "enrollment") == 0 ) {
			if (strcmp(stu -> enrollment, condition -> val) == 0){
				return true;	
			}
			return false;	
		}
	} 
	return false;
}
#endif

#ifndef TEST_EXECUTE
/*
	1.This function takes two arguments:
		1. pointer to `StudentDatabase` object.
		2. pointer to `ParseResult` object.	
	2. This function returns a pointer to `ExecuteResult` object.
	3. The `ExecuteResult` object has two fields: First, an array of matched Student's index. Second, length of the array. 
*/
ExecuteResult * ExecuteQuery(StudentDatabase * db, ParseResult * res) {

	int conditionFail;
	int count;
	ExecuteResult * studList = malloc(sizeof(ExecuteResult));
	studList -> arr = malloc(sizeof(int)*200);
	
	if(strcmp(res -> logic, "AND") ==0){
		for(int i=0;i<(db->number);i++){
			conditionFail = 0;
			for(int j = 0; j < res -> condition_num;j++){
				if(Compare(db -> students[i], res -> conditions[j]) == false){
				conditionFail = conditionFail + 1;
				break;
				}
			}
			if (conditionFail == 0){
				studList -> arr[count] = db -> students[i] -> id;
				count = count + 1;
			}
		}
	}

	else{
		for(int i=0;i<(db -> number);i++){
			for(int j = 0;	j< res -> condition_num;j++){
				if(Compare(db -> students[i], res -> conditions[j]) == true){
					studList -> arr[count] = db -> students[i] -> id;
					count++;
				}
			}
		}
	}
	studList -> len = count;

	return studList;
}
#endif

#ifndef TEST_FREEPARSE
/*
	Free the allocated memory in ParseResult object.
*/
void FreeParseResult(ParseResult * res) {
	for(int i = 0; i <(res -> condition_num); i++) {
                free(res -> conditions[i]);
                }

        free(res -> conditions);
        //not sure if the above line is necessary
	free(res -> field);
	free(res);

}
#endif

#ifndef TEST_FREEEXECUTE
/*
	Free the allocated memory in ExecuteResult object.
*/
void FreeExecuteResult(ExecuteResult * res) {
	free(res -> arr);
	free(res);
}
#endif

#ifndef TEST_WRITE

/*
	1. Only write selected fields of students who meet specified conditions to output file.
	2. Print error message "Failed to open file\n" to monitor if the fopen fail. You should not have output file generated if fopen fail. 
	3. Follow this format "field:%s ", or "field:%d ". Add "\n" when you finish writing the selected fileds of a student. Please look at the files in `expected` folder to make sure you understand the format. We will use diff to test. Make sure your output format is correct.
	4. You should write the info of each student in the following order if that fields are selected. id -> name -> major -> year -> enrollment -> age.
	5. The order in selected fields does not affect the order you write to the file.  e.g., `SELECT id name WHERE ...` and `SELECT name id WHERE ...` should have the same output file.  
*/
void WriteDb(StudentDatabase * db, SelectedField * info, ExecuteResult * execute_res, char * filename) {

	FILE * fp = fopen(filename, "w");
        if (fp == NULL){
                printf("Fail to open a file\n");
                return;
        }

/*	int i=0;
	for (i= 0; i< execute_res->;i++){
	    if(
		if(info-> id = true){
			fprintf(filename, "id:%d ", db-> student[i] ->id);
		}	
		if(info-> name = true){
                        fprintf(filename, "name:%s ", db-> student[i] -> name);
                } 
		if(info-> major = true){
                        fprintf(filename, "major:%s ", db-> student[i] -> major);
                } 
		if(info-> year = true){
                        fprintf(filename, "year:%s ", db-> student[i] -> year);
                } 
		if(info-> enrollment = true){
                        fprintf(filename, "enrollment:%s ", db-> student[i] -> enrollment);
                } 
		if(info-> age = true){
                        fprintf(filename, "age:%d ", db-> student[i] ->age);
                } 
	}

		

*/
}

#endif
