#include <stdlib.h>
#include <stdio.h>
#include "pa06.h"

/*
	Complete the main function and meet the following specifications.
	In this main function you will
	1. Use `Connect()` function to connect to database.txt. You should hard code `database.txt` in `Connect()` function.
	2. Pass `argc`, `argv` to `ParseQuery()` and get the parse results.
	3. Call `ExecuteQuery()` function to get the result of students who meet the condition we specify in the query. 
	4. Use `WriteDb()` to write to file named `output.txt`. You should hard code this name. 
	5. Free memory.
 */
int main(int argc, char **argv) {

	//part 1 connect to database
	StudentDatabase * db = Connect("database.txt");

	//part 2 call parsequery
	ParseResult * pRes = ParseQuery(argc,argv);
	
	//part 3 call executequery
	ExecuteResult * eRes = ExecuteQuery(db, pRes);
	
	//part 4 write db
	WriteDb(db, pRes -> field, eRes, "output.txt");
	
	//part 5 free memory
	Close(db);
	FreeParseResult(pRes);
	FreeExecuteResult(eRes);
	
    return EXIT_SUCCESS;
}

