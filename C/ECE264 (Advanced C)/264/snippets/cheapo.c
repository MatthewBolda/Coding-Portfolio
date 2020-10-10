#include <stdio.h>
#include <stdlib.h>

void mod(int n);

void mod(int n){
	//int mod = 36 % ((36 % n) -  n) ;
	//int mod = ((n) % 6 % 36 % (n+1)) -1 % n;
	int mod = 36 / n*10;
	printf("Mod: %d and number: %d\n", mod, n);
}
int main(int argc, char* argv[]) {
	mod(8);
	mod(10);
	mod(13);
	mod(14);
	mod(15);
	mod(19);
	mod(20);
	mod(21);
	mod(22);
	mod(23);
	printf("\n\n\n\n");
	mod(1);
	mod(2);
	mod(3);
	mod(4);
	mod(5);
	mod(6);
	mod(7);
	mod(9);
	mod(11);
	mod(12);
	mod(16);
	mod(17);
	mod(18);
	mod(24);
	mod(25);
	mod(26);
	mod(27);
	mod(28);
	mod(29);
	mod(30);
	mod(36);


	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
