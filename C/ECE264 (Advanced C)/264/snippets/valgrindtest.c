#include <stdlib.h>
#include <stdio.h>

int main() {
	char* s = malloc(sizeof(*s) * 3);
	s[0] = 'A';
	s[1] = 'B';
	s[2] = 'C';
	printf("%s", s);
	//if(s[3] == 'D') {
	//	printf("%c", s[3]);
	//}
	free(s);
}
