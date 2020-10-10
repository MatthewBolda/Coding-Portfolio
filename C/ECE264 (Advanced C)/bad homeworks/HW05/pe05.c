#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pe05.h"

#ifndef TEST_CPRNAME
static int CompareByName(const void * s1, const void * s2) {
	
	const Student * * ptr1 = (const Student * *) s1;
	const Student * * ptr2 = (const Student * *) s2;
	
	const Student * str1 = * ptr1;
	const Student * str2 = * ptr2;

	return strcmp(str2 -> name,str1-> name);
	
}
#endif

#ifndef TEST_NAME
void SortDatabaseByName(StudentDatabase * studb) {
	// use qsort to sort the database.
	qsort(studb -> students, studb -> number, sizeof(Student *),CompareByName);
}
#endif

#ifndef TEST_CPRAGE
static int CompareByAge(const void * s1, const void * s2) {
	const Student * * ptr1 = (const Student * *) s1; 
        const Student * * ptr2 = (const Student * *) s2;

        const Student * val1 = * ptr1;
        const Student * val2 = * ptr2;

        if (val1 < val2){
                return -1;
        }
        if(val1 == val2){
                return 0;
        }
        return 1;

}
#endif

#ifndef TEST_AGE
void SortDatabaseByAge(StudentDatabase * studb) {
	// use qsort to sort the database.
	qsort(studb -> students, studb -> number, sizeof(Student *),CompareByAge);
}
#endif

#ifndef TEST_WRITE
int WriteDatabase(char *filename, StudentDatabase * db) {
	// Write the info storing in db to filename
	FILE * fp = fopen(filename, "w");
	if (fp == NULL){
		printf("Fail to open a file\n");
		return 0;
	}

	for(int x=0; x < db -> number;x++){
		printf("ID:%d, Name:%s, Major:%s, Enrollment:%s, Year:%s, Age:%d\n",db -> students[x] -> id, db -> students[x]  -> name, db -> students[x]  -> major, db -> students[x]  -> enrollment,db -> students[x]  -> year, db -> students[x]  -> age);
		}
	fclose(fp);
	return 1;	


/*	int stu_num = studb -> number;
        for (int idx = 0; idx < stu_num; idx ++) {
                Student * stu = studb -> students[idx];
                PrintStudent(stu);
*/
}
#endif

