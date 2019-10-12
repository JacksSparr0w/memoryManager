#include <stddef.h> 
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <Windows.h>

#include <stdio.h>  
#include <assert.h>
#include "mmemory.h"

void init_invalid_parameters() {
	int err = _init(-1, 31);
	assert(INVALID_PARAMETERS == err);
}

void init_successful_implementation() {
	int err = _init(1, 20);
	assert(SUCCESS == err);
}

void malloc_invalid_parameters() {
	int err = _malloc(NULL, 10);
	assert(INVALID_PARAMETERS == err);
}

void malloc_lack_of_memory() {
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;
	list->head = NULL;
	list->last = NULL;

	VA va1;
	VA va2;
	_malloc(&va1, 10);
	int err = _malloc(&va2, 4);
	free(list);
	assert(LACK_OF_MEMORY == err);
}

void malloc_succesful_implementation() {
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;
	list->head = NULL;
	list->last = NULL;

	VA va1;
	int err = _malloc(&va1, 7);
	free(list);

	assert(SUCCESS == err);
}

void free_invalid_parameters() {
	VA va1 = (VA)malloc(sizeof(VA));
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;
	list->head = NULL;
	list->last = NULL;

	int err = _free(va1);
	free(list);

	assert(INVALID_PARAMETERS == err);
}

void free_successful_implementation() {
	VA va1 = (VA)malloc(sizeof(VA));
	int size = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = size;
	list->size_free_memory = size;
	list->head = NULL;
	list->last = NULL;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;
	list->head = memoryBlock;
	list->last = memoryBlock;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;
	memoryBlock->va = va1;

	list->size_free_memory -= memoryBlock->size;

	int err = _free(va1);
	free(memoryBlock);
	free(list);

	assert(SUCCESS == err);
}

void read_invalid_parameters_negative() {
	int size = -5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA* pBuffer = (VA*)malloc(size * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, size);
	free(memoryBlock);
	free(list);

	assert(INVALID_PARAMETERS == err);
}

void read_invalid_parameters_null_va() {
	int size = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = NULL;
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA pBuffer = (VA)malloc(size * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, size);
	free(memoryBlock);
	free(list);

	assert(INVALID_PARAMETERS == err);
}

void read_invalid_parameters_null_buffer() {
	int size = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = NULL;
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA pBuffer = NULL;
	int err = _read(memoryBlock->va, pBuffer, size);
	free(memoryBlock);
	free(list);
	assert(INVALID_PARAMETERS == err);
}

void read_lack_of_memory() {
	int bigSize = 10;
	int smallSize = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = bigSize;
	list->size_free_memory = bigSize;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(bigSize * sizeof(VA));
	memoryBlock->size = bigSize;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA pBuffer = (VA)malloc(smallSize * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, smallSize);
	free(pBuffer);
	free(memoryBlock);
	free(list);

	assert(LACK_OF_MEMORY == err);
}

void read_unknown_error() {
	int size = 5;
	list = NULL;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	VA pBuffer = (VA)malloc(size * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, size);
	free(memoryBlock);
	free(pBuffer);
	assert(UNKNOWN_ERROR == err);
}

void read_successful_implemenation() {
	int size = 5;
	list = (List*)malloc(1 * sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	VA pBuffer = (VA)malloc(size * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, size);
	free(pBuffer);
	free(memoryBlock);
	free(list);
	assert(SUCCESS == err);
}

void read_address_ariphmetics() {
	int size = 5;
	list = (List*)malloc(1 * sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(size * sizeof(VA));
	memoryBlock->va = "chars";
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	VA pBuffer = (VA)malloc(size * sizeof(VA));
	for (int i = 0; i < size; i++) {
		pBuffer[i] = 'a' + i;
	}
	int err = _read(memoryBlock->va, pBuffer, size);
	free(pBuffer);
	free(memoryBlock);
	free(list);
	assert(SUCCESS == err);
}

void write_invalid_parameters() {
	int bigSize = 10;
	int smallSize = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = smallSize;
	list->size_free_memory = smallSize;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(smallSize * sizeof(VA));
	memoryBlock->size = smallSize;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = NULL;
	list->last = NULL;
	//list->size_free_memory -= memoryBlock->size;
	VA pBuffer = (VA)malloc(bigSize * sizeof(VA));
	int err = _write(memoryBlock->va, pBuffer, smallSize);
	free(pBuffer);
	free(memoryBlock);
	free(list);
	assert(INVALID_PARAMETERS == err);
}

void write_unknown_error() {
	int size = 5;
	list = NULL;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	VA pBuffer = (VA)malloc(size * sizeof(VA));
	int err = _write(memoryBlock->va, pBuffer, size);
	free(pBuffer);
	free(memoryBlock);
	assert(UNKNOWN_ERROR == err);
}

void write_lack_of_memory() {
	int bigSize = 10;
	int smallSize = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = smallSize;
	list->size_free_memory = smallSize;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(smallSize * sizeof(VA));
	memoryBlock->size = smallSize;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA pBuffer = (VA)malloc(bigSize * sizeof(VA));
	int err = _write(memoryBlock->va, pBuffer, bigSize);
	free(pBuffer);
	free(memoryBlock);
	free(list);
	assert(LACK_OF_MEMORY == err);
}

void write_succeslul_implementation() {
	int bigSize = 10;
	int smallSize = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = bigSize;
	list->size_free_memory = bigSize;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(bigSize * sizeof(VA));
	memoryBlock->size = bigSize;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA pBuffer = (VA)malloc(smallSize * sizeof(VA));
	int err = _write(memoryBlock->va, pBuffer, smallSize);
	free(pBuffer);
	free(memoryBlock);
	free(list);
	assert(SUCCESS == err);
}

void write_address_ariphmetic() {
	int bigSize = 10;
	int smallSize = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = bigSize;
	list->size_free_memory = bigSize;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(bigSize * sizeof(VA));
	memoryBlock->size = bigSize;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA pBuffer = (VA)malloc(smallSize * sizeof(VA));
	for (int i = 0; i < smallSize; i++) {
		pBuffer[i] = 'a' + i;
	}
	int err = _write(memoryBlock->va, pBuffer+2, smallSize);
	free(pBuffer);
	free(memoryBlock);
	free(list);

	assert(SUCCESS == err);
}

void load_test() {
	FILE* result;
	fopen_s(&result, "result.txt", "w");
	fprintf(result, "errors\tIteration\t\n");
	int originSize = 600;
	int size = 2;
	int numberOfBlocks = 120;
	VA adresses[120];
	srand(time(NULL));
	_init(1, originSize);
	int cycles = 100;
	int cycle = 0;
	while (cycle < cycles){
		int errors = 0;
		for (int j = 0; j < numberOfBlocks; j++) {
			size = 2 + rand() % 4;
			if (_malloc(&adresses[j], size) == 1) {
				errors++;
			}
		}
		double procOfErrors = 100.0 * errors / numberOfBlocks;
		fprintf(result, "%0.2f\t\t\t%d\n", procOfErrors, cycle + 1);
		for (int j = 0; j < numberOfBlocks; j) {
			_free(adresses[j]);
			j += 1 + rand() % 2;

		}
		cycle++;
	}
	for (int i = 0; i < 120; i++) {
		_free(adresses[i]);
	}

	fclose(result);
}

void tests() {
	init_invalid_parameters();
	init_successful_implementation();

	malloc_invalid_parameters();
	malloc_lack_of_memory();
	malloc_succesful_implementation();

	read_successful_implemenation();
	read_lack_of_memory();
	read_unknown_error();
	read_invalid_parameters_negative();
	read_invalid_parameters_null_va();
	read_invalid_parameters_null_buffer();
	read_address_ariphmetics();

	free_invalid_parameters();
	free_successful_implementation();

	write_invalid_parameters();
	write_unknown_error();
	write_lack_of_memory();
	write_succeslul_implementation();
	write_address_ariphmetic();

	load_test();
}

int main(void)
{
	tests();
	printf("all tests passed");
}