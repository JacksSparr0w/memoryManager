#include <stdio.h>  
#include <assert.h>
#include "mmemory.h"

void init_invalid_parameters() {
	int err = _init(-1, 31);
	assert(-1 == err);
}

void init_successful_implementation() {
	int err = _init(1, 20);
	assert(0 == err);
}

void malloc_invalid_parameters() {
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;
	list->head = NULL;
	list->last = NULL;
	
	int err = _malloc(NULL, 10);
	assert(-1 == err);
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
	assert(-2 == err);
}

void malloc_unknown_error() {
	free(list);
	list = NULL;

	VA va1;
	int err = _malloc(&va1, 10);
	assert(1 == err);
}

void malloc_succesful_implementation() {
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;
	list->head = NULL;
	list->last = NULL;

	VA va1;
	int err = _malloc(&va1, 7);
	assert(0 == err);
}

void free_invalid_parameters() {
	VA va1 = (VA)malloc(sizeof(VA));
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;
	list->head = NULL;
	list->last = NULL;

	int err = _free(va1);
	assert(-1 == err);
}

void free_successful_implementation() {
	VA va1 = (VA)malloc(sizeof(VA));
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;
	list->head = NULL;
	list->last = NULL;

	MemoryBlock* block = newMemoryBlock(7);
	list->head = block;
	list->last = block;
	block->next = NULL;
	block->previous = NULL;

	int err = _free(va1);
	assert(0 == err);
}

void read_invalid_parameters_negative() {
	int size = -5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA*)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA* pBuffer = (VA*)malloc(size * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, size);
	assert(-1 == err);
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
	VA* pBuffer = (VA*)malloc(size * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, size);
	assert(-1 == err);
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
	VA* pBuffer = NULL;
	int err = _read(memoryBlock->va, pBuffer, size);
	assert(-1 == err);
}

void read_lack_of_memory() {
	int bigSize = 10;
	int smallSize = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = bigSize;
	list->size_free_memory = bigSize;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA*)malloc(bigSize * sizeof(VA));
	memoryBlock->size = bigSize;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA* pBuffer = (VA*)malloc(smallSize * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, smallSize);
	assert(-2 == err);
}

void read_unknown_error() {
	int size = 5;
	list = NULL;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA*)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	VA* pBuffer = (VA*)malloc(size * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, size);
	assert(1 == err);
}

void read_successful_implemenation() {
	int size = 5;
	list = (List*)malloc(1 * sizeof(List));
	list->size_memory = 10;
	list->size_free_memory = 10;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA*)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	VA* pBuffer = (VA*)malloc(size * sizeof(VA));
	int err = _read(memoryBlock->va, pBuffer, size);
	assert(0 == err);
}

void write_invalid_parameters() {
	int bigSize = 10;
	int smallSize = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = smallSize;
	list->size_free_memory = smallSize;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA*)malloc(smallSize * sizeof(VA));
	memoryBlock->size = smallSize;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = NULL;
	list->last = NULL;
	//list->size_free_memory -= memoryBlock->size;
	VA* pBuffer = (VA*)malloc(bigSize * sizeof(VA));
	int err = _write(memoryBlock->va, pBuffer, smallSize);
	assert(-1 == err);
}

void write_unknown_error() {
	int size = 5;
	list = NULL;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA*)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	VA* pBuffer = (VA*)malloc(size * sizeof(VA));
	int err = _write(memoryBlock->va, pBuffer, size);
	assert(1 == err);
}

void write_lack_of_memory() {
	int bigSize = 10;
	int smallSize = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = smallSize;
	list->size_free_memory = smallSize;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA*)malloc(smallSize * sizeof(VA));
	memoryBlock->size = smallSize;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA* pBuffer = (VA*)malloc(bigSize * sizeof(VA));
	int err = _write(memoryBlock->va, pBuffer, bigSize);
	assert(-2 == err);
}

void write_succeslul_implementation() {
	int bigSize = 10;
	int smallSize = 5;
	list = (List*)malloc(sizeof(List));
	list->size_memory = bigSize;
	list->size_free_memory = bigSize;

	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA*)malloc(bigSize * sizeof(VA));
	memoryBlock->size = bigSize;
	memoryBlock->next = NULL;
	memoryBlock->previous = NULL;

	list->head = memoryBlock;
	list->last = memoryBlock;
	list->size_free_memory -= memoryBlock->size;
	VA* pBuffer = (VA*)malloc(smallSize * sizeof(VA));
	int err = _write(memoryBlock->va, pBuffer, smallSize);
	assert(0 == err);
}

void tests() {
	init_invalid_parameters();
	init_successful_implementation();

	malloc_invalid_parameters();
	malloc_unknown_error();
	malloc_lack_of_memory();
	malloc_succesful_implementation();

	read_successful_implemenation();
	read_lack_of_memory();
	read_unknown_error();
	read_invalid_parameters_negative();
	read_invalid_parameters_null_va();
	read_invalid_parameters_null_buffer();

	free_invalid_parameters();
	free_successful_implementation();

	write_invalid_parameters();
	write_unknown_error();
	write_lack_of_memory();
	write_succeslul_implementation();
}

int main(void)
{
	tests();
	printf("all tests passed");
}