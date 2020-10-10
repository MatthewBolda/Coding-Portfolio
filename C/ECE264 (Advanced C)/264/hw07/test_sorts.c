#include <stdio.h>
#include <stdlib.h>
#include "sorts.h"

int main(int argc, char* argv[]) {

	int test1 [] = {7,5,6,3,4,1,2};
	int array_size = sizeof(test1) / sizeof(test1[0]);
	merge_sort_array(test1 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test1[i]);
	}
	printf("\n");

	int test2 [] = {4,3,2,1,4,3,2,1,4,4,4};
	array_size = sizeof(test2) / sizeof(test2[0]);
	merge_sort_array(test2 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test2[i]);
	}
	printf("\n");

	int test3 [] = {51235, 78510, -1, 0, -4467419, 2147483647, -2147483648};
	array_size = sizeof(test3) / sizeof(test3[0]);
	merge_sort_array(test3 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test3[i]);
	}
	printf("\n");
	
	int test4 [] = {1,1,1,1};
	array_size = sizeof(test4) / sizeof(test4[0]);
	merge_sort_array(test4 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test4[i]);
	}
	printf("\n");
	
	
	int test5 [] = {5,3,6,4,1,8};
	array_size = sizeof(test5) / sizeof(test5[0]);
	tree_sort_array(test5 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test5[i]);
	}
	printf("\n");
	
	int test6 [] = {51235, 78510, -1, 0, -4467419, 2147483647, -2147483648};
	array_size = sizeof(test6) / sizeof(test6[0]);
	tree_sort_array(test6 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test6[i]);
	}
	printf("\n");

	int test7 [] = {4,3,2,1,4,3,2,1,4,4,4};
	array_size = sizeof(test7) / sizeof(test7[0]);
	tree_sort_array(test7 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test2[i]);
	}
	printf("\n");

	int test8 [] = {7,5,6,3,4,1,2};
	array_size = sizeof(test8) / sizeof(test8[0]);
	quick_sort_array(test8 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test8[i]);
	}
	printf("\n");

	int test9 [] = {4,3,2,1,4,3,2,1,4,4,4};
	array_size = sizeof(test9) / sizeof(test9[0]);
	quick_sort_array(test9 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test9[i]);
	}
	printf("\n");

	int test10 [] = {51235, 78510, -1, 0, -4467419, 2147483647, -2147483648};
	array_size = sizeof(test10) / sizeof(test10[0]);
	quick_sort_array(test10 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test10[i]);
	}
	printf("\n");
	
	int test11 [] = {1};
	array_size = sizeof(test11) / sizeof(test11[0]);
	quick_sort_array(test11 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test11[i]);
	}
	printf("\n");
	
	int test12 [] = {1};
	array_size = sizeof(test12) / sizeof(test12[0]);
	quick_sort_array(test12 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test12[i]);
	}
	printf("\n");

	int test13 [] = {1};
	array_size = sizeof(test13) / sizeof(test10[3]);
	quick_sort_array(test13 , array_size);
	for(int i = 0; i < array_size; i++){
		printf("%d ", test13[i]);
	}
	printf("\n");
	// following three tests are for size = 0
	int random_array [1] = {1};   // error when compiling zero-size array so just 1
	int zero_size = 0;
	tree_sort_array(random_array,zero_size);
	merge_sort_array(random_array,zero_size);
	quick_sort_array(random_array,zero_size);


	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
