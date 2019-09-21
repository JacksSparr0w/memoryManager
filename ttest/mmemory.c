#include "mmemory.h"
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

List* list;

int _malloc(VA* ptr, size_t szBlock)
{
	if (ptr == NULL || szBlock == NULL) {
		return INVALID_PARAMETERS;
	}
	if (list == NULL) {
		return UNKNOWN_ERROR;
	}
	if (list->size_free_memory < szBlock) {
		return LACK_OF_MEMORY;
	}

	MemoryBlock* block = newMemoryBlock(szBlock);
	if (list->head == NULL) {
		list->head = block;
		list->last = block;
		block->next = NULL;
		block->previous = NULL;
	}
	else {
		block->next = NULL;
		block->previous = list->last;
		list->last->next = block;
		list->last = block;
	}
	list->size_free_memory -= szBlock;
	*ptr = block->va;
	return SUCCESS;
}

int _free(VA ptr)
{
	if (list == NULL) {
		return UNKNOWN_ERROR;
	}
	MemoryBlock* block = findBlock(ptr);
	if (ptr == NULL || block == NULL) {
		return INVALID_PARAMETERS;
	}

	if (block->next && block->previous) {
		block->previous->next = block->next;
		block->next->previous = block->previous;
	}
	if (block->next && !block->previous) {
		block->next->previous = NULL;
		list->head = block->next;
	}
	if (!block->next && block->previous) {
		block->previous->next = NULL;
		list->last = block->previous;
	}
	if (!block->next && !block->previous) {
		list->head = NULL;
		list->last = NULL;
	}
	free(ptr);
	list->size_free_memory += block->size;
	return SUCCESS;
}

int _read(VA ptr, void* pBuffer, size_t szBuffer)
{
	return 0;
}

int _write(VA ptr, void* pBuffer, size_t szBuffer)
{
	return 0;
}

int _init(int n, int szPage)
{
	if (n <= 0 || szPage <= 0) {
		return INVALID_PARAMETERS;
	}
	if (list != NULL) {
		free(list);
	}
	list = (List*)malloc(sizeof(List));
	if (list == NULL) {
		return UNKNOWN_ERROR;
	}
	int size = n * szPage;
	list->size_memory = size;
	list->size_free_memory = size;
	list->head = NULL;
	list->last = NULL;
	return SUCCESS;
}

MemoryBlock* newMemoryBlock(size_t size) {
	MemoryBlock* memoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	memoryBlock->va = (VA)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	return memoryBlock;
}

MemoryBlock* findBlock(VA ptr) {
	MemoryBlock* block = list->head;
	while (block != NULL) {
		if (block->va = ptr) {
			return block;
		}
		block = block->next;
	}
	return NULL;
}
