#include <stdio.h>
#include <stdlib.h>
int f1(int a)
{
	int b = a + 1;
	return b;
}

int f2(int a)
{
	if (a==0)
	{
		return f1(a+2);
	}
	
	int b = a;
	int c = 0;
	c = b +f2(a-1);
	return c;
}

int main(int argc, char * * argv)
{
	int a = 0;
	a = f2(3);
	printf("a = %d\n",a);
	return EXIT_SUCCESS;
}

