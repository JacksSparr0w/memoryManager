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

	MemoryBlock* block = list->head;
	if (block == NULL) {
		MemoryBlock* newBlock = newMemoryBlock(szBlock);
		list->head = newBlock;
		list->last = newBlock;
		newBlock->next = NULL;
		newBlock->previous = NULL;
		newBlock->startByte = 0;
		list->size_free_memory -= szBlock;
		*ptr = newBlock->va;
		return SUCCESS;
	}

	if (block->startByte >= szBlock) {
		MemoryBlock* newBlock = newMemoryBlock(szBlock);
		list->head = newBlock;
		newBlock->next = block;
		block->previous = newBlock;
		newBlock->previous = NULL;
		newBlock->startByte = 0;
		list->size_free_memory -= szBlock;
		*ptr = newBlock->va;
		return SUCCESS;
	}
	block = block->next;
	while (block!= NULL) {
		int freeSpace = block->startByte - block->previous->startByte - block->previous->size;
		if (freeSpace >= szBlock) {
			MemoryBlock* newBlock = newMemoryBlock(szBlock);
			newBlock->next = block;
			newBlock->previous = block->previous;
			block->previous->next = newBlock;
			block->previous = newBlock;
			int startByte = newBlock->previous->startByte + newBlock->previous->size;
			newBlock->startByte = startByte;
			list->size_free_memory -= szBlock;
			*ptr = newBlock->va;
			return SUCCESS;
		}
		block = block->next;
	}
	int freeSpaceAfterLast = list->size_memory - list->last->startByte - list->last->size;
	if (freeSpaceAfterLast >= szBlock) {
		MemoryBlock* newBlock = newMemoryBlock(szBlock);
		newBlock->next = NULL;
		newBlock->previous = list->last;
		list->last->next = newBlock;
		list->last = newBlock;
		int startByte = newBlock->previous->startByte + newBlock->previous->size;
		newBlock->startByte = startByte;
		list->size_free_memory -= szBlock;
		*ptr = newBlock->va;
		return SUCCESS;
	}

	return UNKNOWN_ERROR;
}

int _free(VA ptr)
{
	if (list == NULL) {
		return UNKNOWN_ERROR;
	}
	MemoryBlock* block = findBlock(ptr, 0);
	if (block == NULL) {
		block = findBlock(ptr, 1);
	}
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
	if (list == NULL) {
		return UNKNOWN_ERROR;
	}
	MemoryBlock* block = findBlock(ptr, 0);
	if (ptr == NULL || pBuffer == NULL || szBuffer < 0 || block == NULL) {
		return INVALID_PARAMETERS;
	}
	if (block->size <= szBuffer) {
		memcpy(pBuffer, ptr, block->size);
		return SUCCESS;
	}
	else {
		return LACK_OF_MEMORY;
	}
}

int _write(VA ptr, void* pBuffer, size_t szBuffer)
{
	if (list == NULL) {
		return UNKNOWN_ERROR;
	}

	MemoryBlock* block = findBlock(ptr, 1);
	if (ptr == NULL || pBuffer == NULL || szBuffer < 0 || block == NULL) {
		return INVALID_PARAMETERS;
	}

	if (block->size >= szBuffer) {
		memcpy(ptr, pBuffer, block->size);
		return SUCCESS;
	}
	else {
		return LACK_OF_MEMORY;
	}
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
	memoryBlock->va = (VA*)malloc(size * sizeof(VA));
	memoryBlock->size = size;
	memoryBlock->free = FREE;
	return memoryBlock;
}

MemoryBlock* findBlock(VA ptr, int free) {
	MemoryBlock* block = list->head;
	while (block != NULL) {
		if (block->va == ptr && block->free == free) {
			return block;
		}
		block = block->next;
	}
	return NULL;
}
