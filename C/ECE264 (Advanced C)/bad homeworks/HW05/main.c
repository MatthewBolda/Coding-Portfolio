#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pe05.h"


/*
	Complete the main function and meet the followin specifications of argc and argv.
	1. argc: If argc is not 4, you should return `EXIT_FAILURE` and print "Wrong arguments\n".
	2. argv[1]: name of the database. i.e., database.txt.
	3. argv[2]: should be either "-a", or "-n".
		When argv[2] is "-a", you should sort the database by age in ascending order and write the database to a file and return `EXIT_SUCCESS`.
		When argv[2] is "-n", you should sort the database by name in descending order and write the dataabse to a file and return `EXIT_SUCCESS`. 
		For example, Student "Allen" should come after "Bob".
	  	If argv[2] is neither "-a" nor "-n", you should print "Wrong arguments\n" and return `EXIT_FAILURE`.
	4. argv[3]: This is the output filename. You should write the output of database into this file.
	5. print "Write to db\n" to screen if `WriteDatabase()` work. Otherwise, print "Fail to write to db\n"

*/ 

int main(int argc, char ** argv) {
	// specification 1
	if (argc != 4){
		printf("Wrong arguments\n");
		return EXIT_FAILURE;
		}
	//specification 2 
	StudentDatabase * db = Connect(argv[1]);
	if( db == NULL){
		printf("fail to open database");
		}
	//specification 3 part 1
	if (strcmp(argv[2],"-a") == 0){
		
		SortDatabaseByAge(db);
		Close(db);
		return EXIT_SUCCESS;
		}
	//specification 3 part 2
	else if(strcmp(argv[2],"-n") == 0){
		
		SortDatabaseByName(db);
		Close(db);
		return EXIT_SUCCESS;
		}
	//specification 3 part 3
	else {
		printf("Wrong argument\n");
		Close(db);
		return EXIT_FAILURE;
		}
	//specification 4 and 5
	
	int x = WriteDatabase(argv[3], db);

	if (x == 1){
		printf("Write to db\n");
		}
	if (x == 0){
		printf("Fail to write to db\n");
		}

	Close(db);
	return EXIT_SUCCESS;

}
