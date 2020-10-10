#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pe07.h"




#ifndef INT
bool IsInteger(char* str, int index)
{
	if(isdigit(str[index])){

		if (index == (strlen(str)-1)){
			return true;
		}
		else{
			index = index + 1;
			IsInteger(str, index);
		}		
	}



		return false;

	
}
#endif

#ifndef DOUBLE
bool IsDouble(char* str, int index,bool dot)
{
	int length = strlen(str);
	if(isdigit(str[index])){

		if((str[index] == '.') && !dot && index != 0 &&  index != length-1){
			dot = true;
		}
		else if((str[index] == '.') && dot){
			return false;
		}
		if(index == (strlen(str)-1)){
			return true;
		}
		else{
			index = index + 1;
			IsDouble(str, index, dot);
		}
	}
	
		return false;
		
	
}
#endif

#ifndef VALIDID
bool IsValidIdentifier(char* str, int index)
{
	if(isalpha(str[index]) || isdigit(str[index]) || (str[index] == '_')){
		if(index == (strlen(str) - 1)){
			return true;
		}
		else{
			index = index + 1;
			IsValidIdentifier(str, index);
		}
	}	
	return false;
		

}
#endif

