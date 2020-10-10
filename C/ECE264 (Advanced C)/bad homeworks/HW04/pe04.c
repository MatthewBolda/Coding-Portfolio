#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TEST_STRUCT
#include "pe04.h"
#else
#include "pe04_ans.h"
#endif

#ifndef TEST_CONNECT
/*  
	Complete this function. 
	In this function, you should:
	1. Allocate appripriate memory for database using malloc.
	2. Read the content of file and store it to the StudentDatabase object.
		To read the file, you may want to use the following functions:
			fopen(), fclose(), fgetc(), fscanf(), feof().
		You can use fseek(fptr, 0, SEEK_SET) to go to the beginning of a file. 
	3. You should return a pointer to StudentDatabase object.
	4. If you fail to connect to the database, you should return NULL.
*/
StudentDatabase * Connect(char * filename) {
	StudentDatabase * db = NULL;

	//initialize n lines
	int nline = 0; 
	FILE * fptr = fopen(filename,"r");
	int x = 0;
	do {
		//return NULL if the file cant be opened
		if(fptr == NULL) {		
			return EXIT_SUCCESS;
		}
		x = fseek( fptr, 0, SEEK_SET); //make sure it goes to the beginning of the text file
		//if statement to make sure it goes until the end of the text file
		if (x != EOF){
			if (x == '\n'){
			nline ++; //adds 1 to nline for every new line found in the code
			}
		}


	}while (x != EOF);

	db->number = nline;
	db = malloc(sizeof(StudentDatabase));
	db->students = malloc(sizeof(Student*)*nline);
	
	for(int i = 0; i < nline; i++)
	{
	db->students[i] = malloc(sizeof(Student));
	

	fseek(fptr,0, SEEK_SET);
	for(int count = 0; count < nline; count++){

	fscanf(fptr,"%d",&(db->students[count]->id));

	
	
	int ch = fgetc(fptr);
	
	count = 0;
	
	do{
		db->students[i]->name[count] = ch; 
		count++;
		ch = fgetc(fptr);
	}while(ch != '\n');

	ch = fgetc(fptr);
	count = 0;
        
	do{
       		 db->students[i]->major[count] = ch;
       		 count++;
       		 ch = fgetc(fptr);
        }while(ch != '\n');
	
	ch = fgetc(fptr);
	count = 0;
	
	do{
       		db->students[i]->year[count] = ch;
      		count++;
        	ch = fgetc(fptr);
        }while(ch != '\n');
	
	ch = fgetc(fptr);
	count = 0;
 	
	do{
        	db->students[i]->enrollment[count] = ch;
        	count++;
       		ch = fgetc(fptr);
        }while(ch != '\n');

   fscanf(fptr,"%d",&(db->students[count]->age));
}
}
Close(db);
	return db;
}
#endif

#ifndef TEST_CLOSE
/* 
	You have to complete this function
	Free the memory you allocated in Connect() using free()
	studb is a pointer to the database. 
*/
void Close(StudentDatabase * studb) {

for(int i = 0; i < studb->number; i++){
free(studb->students[i]);
}

free(studb->students);
free(studb);

}
#endif

#ifndef TEST_SEARCH
/* 
	You have to complete this function.
	1. studb is a pointer to StudentDatbase object.
	2. name is the name of the student you are looking for.
	3. If the "name" is in the database, you should return a pointer 
	to the Student object of that student. Otherwise, you should return NULL. 
 */
Student * SearchByName(StudentDatabase * studb, char * name) {

//loop all students
for(int i = 0; i < studb->number; i++){
	if(strcmp(studb -> students[i] -> name, name) == 0){
		return studb -> students[i];	
	}
}
return NULL;
}
#endif
/* Warning: Do not modify or delete codes below this line. */
/* This function prints info of a student. */
void PrintStudent(Student * stu) {
	printf("ID:%d, Name:%s, Major:%s, Enrollment:%s, Year:%s, Age:%d\n",
		stu -> id, stu -> name, stu -> major, stu -> enrollment, stu -> year, stu -> age);
}

/* This function prints all students' info in database. */
void PrintDatabase(StudentDatabase * studb) {
	int stu_num = studb -> number;
	for (int idx = 0; idx < stu_num; idx ++) {
		Student * stu = studb -> students[idx];
		PrintStudent(stu);
	}
}
