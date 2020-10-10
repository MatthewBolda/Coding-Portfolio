#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

void* zero_array_worker(void* arg);
bool zero_array(int* array, int array_length, int num_threads, const char** a_error);
/* TODO:  Create a worker function to go with zero_array(..) */

typedef struct _array_helper {
	int* index;
	size_t size_idx;
} array_helper;

void* zero_array_worker(void* arg){
	array_helper* a_arg = arg;
	for(int i = 0; i < a_arg -> size_idx; i++){
		a_arg -> index[i] = 0;
	}
	return NULL;
}

bool zero_array(int* array, int array_length, int num_threads, const char** a_error) {
	*a_error = ( (array_length > 0) ? *a_error: "error: array_length must be largerr than 0");
	*a_error = ( array != NULL ? *a_error: "error: array must not be NULL");
	num_threads = (array_length > num_threads ? num_threads: array_length);
	if( *a_error != NULL){
		return false;
	}

	pthread_t* threads = malloc(num_threads * sizeof(*threads));
	*a_error = (threads != NULL ? *a_error: "error: malloc failure");
	if( *a_error != NULL){
		return false;
	}
	array_helper* worker_help = malloc( num_threads * sizeof(*worker_help));
	*a_error = (worker_help != NULL ? *a_error: "error: malloc failure");
	if( *a_error != NULL){
		free(threads);
		return false;
	}
	

	size_t num_element_per_thread = array_length / num_threads;
	
	for( int i = 0; i < array_length; i = i + num_element_per_thread){
		int idx = i * num_element_per_thread;
		worker_help[i] = (array_helper) { .index = &array[idx],
											.size_idx = num_element_per_thread };
		if(
			
		pthread_create(&threads[i],NULL, zero_array_worker, &worker_help[i]); 
	}
	/*
	for(int i = 0; i < num_threads; i++){
		int idx = i * num_element_per_thread;	
		worker_help[i] = (array_helper) { .index = &array[idx],
										  .size_idx = num_element_per_thread };
		bool create_success = pthread_create(&threads[i],NULL, zero_array_worker, &worker_help[i]); 
		if(!create_success){
			*a_error = "error: thread could not be created";
		}
	}
	*/	
	for(int j = 0; j < 2; j++){
		pthread_join(threads[j], NULL);
	}
	
	free(threads);
	//free(worker_help);
	return true;
}

int main(int argc, char* argv[]) {
	int num_threads = 4;

	int array[16] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	int array_length = sizeof(array)/sizeof(*array);

	for(int i=0; i<array_length; i++) {
		assert(array[i] == 2);
	}

	const char* error = NULL;
	bool did_succeed = zero_array(array, array_length, num_threads, &error);
	if(did_succeed) {
		for(int i=0; i<array_length; i++) {
			printf("%d", array[i]);
			//assert(array[i] == 0);
		}
		return EXIT_SUCCESS;
	}
	else {
		fprintf(stderr, "%s\n", error);
		return EXIT_FAILURE;
	}
}

// Updated 4/17/2019 7:52 PM:  Error message should be on the DATA SEGMENT.

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
