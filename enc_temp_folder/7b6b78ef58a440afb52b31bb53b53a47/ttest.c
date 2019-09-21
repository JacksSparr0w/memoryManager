#include <stdio.h>  
#include <assert.h>
#include "mmemory.h"

void init_invalid_parameters() {
	int err;
	err = _init(-1, 31);
	assert(-1 == err);
}

void init_successful_implementation() {
	int err;
	err = _init(1, 20);
	assert(0 == err);
}

void malloc_invalid_parameters() {
	int err;
	_init(1, 10);
	err = _malloc(NULL, 10);
	assert(-1 == err);
}

void malloc_lack_of_memory() {
	int err;
	_init(1, 10);
	VA va1;
	VA va2;
	_malloc(&va1, 10);
	err = _malloc(&va2, 4);
	assert(-2 == err);
}

void malloc_unknown_error() {
	int err;
	VA va1;
	err = _malloc(&va1, 10);
	assert(1 == err);
}

void malloc_succesful_implementation() {
	int err;
	VA va1;
	_init(1, 10);
	err = _malloc(&va1, 7);
	assert(0 == err);
}

void free_invalid_parameters() {
	int err;
	VA va1 = (VA)malloc(sizeof(VA));
	_init(1, 10);
	err = _free(va1);
	assert(-1 == err);
}

void init_test() {
	init_invalid_parameters();
	init_successful_implementation();
}

void malloc_test() {
	malloc_unknown_error();
	//malloc_lack_of_memory();
	//malloc_invalid_parameters();
	//malloc_succesful_implementation();
}

void free_test() {
	free_invalid_parameters();
}

int main(void)
{
	malloc_test();
	//init_test();
	//free_test();

	return 0;
}