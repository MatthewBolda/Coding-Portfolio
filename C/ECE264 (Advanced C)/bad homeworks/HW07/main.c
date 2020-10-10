#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "pe07.h"
#define MAX_LENGTH 100 //The maximum length of string


int main(int argc, char *argv[]) 
{
			
	FILE * fptr;
	fptr = fopen(argv[1], "r");
	
	if (fptr == NULL){	
		return EXIT_FAILURE;
	}
	
	/*
	int count = 0;
	
	while(fgets(*str, MAX_LENGTH, fptr (!= NULL)){
		count= count ++;
	}
	fclose(fptr);
	fptr = fopen(argv[1], "r");
	
	*/ //potentially unneeded

	char str [MAX_LENGTH];
	bool dot = false;
	
	while(fscanf(fptr,"%s",str)){
	

		
			bool A = IsInteger(str,0);     // maybe IsInteger(str[test],index);
			bool B = IsDouble(str,0,dot);
			bool C = IsValidIdentifier(str,0);
	
				
			if( A == true){
				printf("Integer\n");
			}
			else if( B == true){
				printf("Double\n");
			}
			else if( C == true){
				printf("Identifier\n");
			}
			else{
				printf("None\n");
			}


	}
	
	fclose(fptr);

	return EXIT_SUCCESS;
}
