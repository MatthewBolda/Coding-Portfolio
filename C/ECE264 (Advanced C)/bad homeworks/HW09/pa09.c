#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pa09.h"


/* Partition function takes the following input:
* 1. Student ** stuArray - takes input as student Array
* 2. int startIndex - start index for the partition
* 3. int endIndex -  end index for the partition
* 4. int (*compare)(const void *, const void *) - this is the compare function that will be passed to partition
*
* Implementation details:
* This function implements the Quicksort functionality. It calls the Partition function to
* get the stuArray such that all the elements on right of the pivot are greater than pivot and
* all elements on the left are smaller than the pivot.
* After getting the partition index from the above call, it calls itself recursively to sort the
* remaining array in the similar fashion.
* Quicksort is in-place sorting algorithm, which means that it doesn't use extra space for storing
* the intermediate or final result.
*/
#ifndef STUDENT_SORT

void StudentQsort(Student** stuArray, int startIndex, int endIndex , CompareFunction compare) {
	//This is Qsort specifically for Students database
	//TODO 1: Check base condition
		if(startIndex <= endIndex)
		{
			return;
		}
	//TODO 2: Get the partition index - i.e call  the partition function and get partition index


	//TODO 3: Call the StudentQsort recursively as mentioned in README

	int part = Partition (stuArray, startIndex, endIndex, compare);
	
	// part + 1 because its at the start and we want it to go to the middle
	StudentQsort(stuArray, part + 1, endIndex, compare);
	
	// part - 1 because its at the end and we want it to go to the middle
	StudentQsort(stuArray, startIndex, part - 1, compare);
	

}

#endif

#ifndef PARTITION
/* Partition function takes the following input:
* 1. Student ** stuArray - takes input as student Array
* 2. int start - start index for the partition
* 3. int end -  end index for the partition
* 4. int (*compare)(const void *, const void *) - this is the compare function that will be passed to partition
*
* Implementation details:
* This function implements the core of Quicksort functionality. It partitions the stuArray such that
* all the elements on right of the pivot are greater than pivot and all elements on the left are smaller than the
* the pivot. Pivot should be the last element of the given array without any sorting.
* Quicksort is in-place sorting algorithm, which means that it doesn't use extra space for storing
* the intermediate or final result.
*/
int Partition(Student** stuArray, int start, int end , CompareFunction compare){

	//TODO 1. Take the pivot to be the last element of the stuArray
	int left = start;
	int right = end -1;
	Student * pivot = stuArray[end];
		

	//TODO 2. Print the pivot by calling PrintStudent function
	PrintStudent(pivot);
	
	//TODO 3. Initialize the partition Index
	//int partInd;

	//TODO 4: iterate over the whole array to partition the array such that the final array has values
	// smaller than pivot on left side of array and values greater than pivot are on
	// the right side of array. Please see FAQ 7 example for clarification
	// Hint: You will need to use the swap function to do this in-place

	//int c3;
	while( left <= right)
	{
		//compare((const void *)&stuArray[left], (const void *)pivot);
		while(left < end && compare(&stuArray[left], &pivot) > 0){
			left ++;
		}
		//compare((const void *)pivot, (const void *)&stuArray[right]);
		while( start <= right && compare(&pivot, &stuArray[right]) < 0){
			right --;
		
		}
		//c3 = compare((const void *)&stuArray[left],(const void *)&stuArray[right]);
		
		if(left < right){
			SwapStudent(&stuArray[left],&stuArray[right]);
			left ++;
			right --;
		}
		else if(left == right){
			left ++;
			right --;
		}
		else{
			SwapStudent(&stuArray[left],&stuArray[right]);
		}
		
	}
	//TODO 5. return the partition Index - check README FAQ example to see its working
	

	return 0;
}

#endif



#ifndef SWAP_STUDENT
//Following function swaps the elements of the struct type Student
void SwapStudent(Student** stu1, Student** stu2){
	//TODO 1. Initialize tmp variable to store one of the student value
	//TODO 2. implement the swap
	//create temps

/*	Student ** tmp1 = stu1;
	Student ** tmp2 = stu2;
	// swap the students
	stu1 = tmp2;
	stu2 = tmp1;
*/
	Student * temp = *stu1;
	*stu1 = *stu2;
	*stu2 = temp;

}

#endif



#ifndef CLOSE

void Close(StudentDatabase * studb) {
	//TODO : Free all the variables
	for(int i = 0;i < studb -> number; i++){
		free(studb -> students[i]);
	}
	//free(studb); maybe this goes first?
	free(studb -> students);
	free(studb);
	

}


#endif


/*
********* WARNING: DO NO CHANGE THE CODE BELOW THIS LINE ********************
*/


#ifndef SOLUTION_CHECK
void PrintStudent(Student *student){
	if(student != NULL){
		printf("ID:%d, Name:%s, Major:%s, Enrollment:%s, Year:%s, Age:%d\n",
			student -> id, student -> name, student -> major, student -> enrollment, student -> year, student -> age);
	}
}



/*
 * CompareByName function is defined to compare elements by name of any two students in database
 * Input is Student datastructure with one of the field as Name
 * */

static int CompareByName(const void * s1, const void * s2) {

	// get the address of the array
	const Student ** s1_array =  (const Student **) s1;
	const Student ** s2_array =  (const Student **) s2;
	// get the element
	const Student * v1 = *s1_array;
	const Student * v2 = *s2_array;
	return strcmp(v2 -> name, v1 -> name);
}


void SortDatabaseByName(StudentDatabase * studb) {
	if (studb != NULL) {
		StudentQsort(studb -> students, 0, studb -> number-1, CompareByName);
	}
}

/*
 * CompareByAge function is defined to compare elements by age of any two students in database
 * Input is Student datastructure with one of the field as Name
 * */

static int CompareByAge(const void * s1, const void * s2) {

	// get the address of the array
	const Student ** s1_array = (const Student **) s1;
	const Student ** s2_array = (const Student **) s2;

	// get the element
	const Student * v1 = *s1_array;
	const Student * v2 = *s2_array;
	return (v2-> age - v1 -> age);

}


void SortDatabaseByAge(StudentDatabase * studb) {
	if (studb != NULL) {
		StudentQsort(studb -> students, 0, studb -> number-1, CompareByAge);
	}
}
#endif
