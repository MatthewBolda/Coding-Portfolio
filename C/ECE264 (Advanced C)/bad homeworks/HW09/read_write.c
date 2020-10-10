#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pa09.h"

#ifndef CONNECT

StudentDatabase * ReadDatabase(char * filename) {
	StudentDatabase * db = NULL;
	//TODO 1. Open the file in read and binary format
	
	FILE * fpin = fopen(filename, "r");
	if (fpin == NULL){
		return NULL;
		}
	
	
	//TODO 2. Allocate memory for db
	// db = malloc(sizeof(*db));
	db = malloc(sizeof(StudentDatabase));
	if (db == NULL){
		fclose(fpin);
		return NULL;
	}

	
	//TODO 3. Find number to entries of Student structure
	fseek(fpin, 0, SEEK_END);
	int length = ftell(fpin);
	int num = length / sizeof(Student);
	//fseek(fpin, 0, SEEK_SET);

	//TODO 4. Set the value of db->number to the number of students
	db -> number = num;
	db -> students = malloc(sizeof( Student *) * num);
	// TODO 5. Allocate the memory for the db->students structure
	for(int i = 0; i < num; i++){	
		db -> students[i] = malloc(sizeof(Student));
	}

	//TODO 6. Read the file again from start and assign value to each student in the data base	
	fseek(fpin, 0, SEEK_SET);
	for(int x = 0; x < num; x++){
		fread(db -> students[x], sizeof(Student), 1, fpin);
		}

 	
	//close the file
	fclose(fpin);

	//return the StudentDatabase
	return db;
}

#endif

#ifndef WRITE_DB
//This function is to write to a file in binary format
int WriteDatabase(char *filename, StudentDatabase * db) {
	//TODO 1. Open the file in write and binary format
	FILE * fpout = fopen(filename, "w+");

	if (fpout == NULL){
                return 0;
                }
	
	//TODO 2. check if the StudentDatabase is not null
	if(db == NULL){
		return 0;
	}
	
 
	
	//TODO 3. write into the file and if not possible return 0

	// num is tester to see if possible
	int num;
	for( int i = 0; i < db -> number; i++){
	
		num = fwrite(db -> students[i], sizeof(Student), 1, fpout);
	
	}
	if(num == 0){
		return 0;
	}
			
	return 1;


}
#endif
