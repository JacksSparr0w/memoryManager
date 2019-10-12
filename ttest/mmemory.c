#include "mmemory.h"
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

List* list;

int _malloc(VA* ptr, size_t szBlock)
{
	if (ptr == NULL || &szBlock == NULL) {
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
		size_t freeSpace = block->startByte - block->previous->startByte - block->previous->size;
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
	size_t freeSpaceAfterLast = list->size_memory - list->last->startByte - list->last->size;
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
	if (list == NULL) {
		return UNKNOWN_ERROR;
	}

	if (ptr == NULL || pBuffer == NULL || szBuffer < 0) {
		return INVALID_PARAMETERS;
	}

	MemoryBlock* block = list->head;
	while (block != NULL) {
		for (int i = 0; i < block->size; i++) {
			VA va = block->va + i;
			if (va == ptr) {
				if (szBuffer >= block->size - i) {
					memcpy(pBuffer, va, szBuffer);
					return SUCCESS;
				}
				else {
					return LACK_OF_MEMORY;
				}
				
			}
		}
		block = block->next;
	}
	return INVALID_PARAMETERS;
}

int _write(VA ptr, void* pBuffer, size_t szBuffer)
{
	if (list == NULL) {
		return UNKNOWN_ERROR;
	}

	if (ptr == NULL || pBuffer == NULL || szBuffer < 0) {
		return INVALID_PARAMETERS;
	}

	MemoryBlock* block = list->head;
	while (block != NULL) {
		for (int i = 0; i < block->size; i++) {
			VA va = block->va + i;
			if (va == ptr) {
				if (szBuffer <= block->size - i) {
					memcpy(va, pBuffer, szBuffer);
					return SUCCESS;
				}
				else {
					return LACK_OF_MEMORY;
				}

			}
		}
		block = block->next;
	}
	return INVALID_PARAMETERS;
}

int _init(int n, int szPage)
{
	if (n <= 0 || szPage <= 0) {
		return INVALID_PARAMETERS;
	}
	/*
	if (list != NULL) {
		free(list);
	}
	*/
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
			for (int i = 0; i < block->size; i++) {
				if (block->va + i == ptr) {
					return block;
				}
			}
		block = block->next;
	}
	return NULL;
}
