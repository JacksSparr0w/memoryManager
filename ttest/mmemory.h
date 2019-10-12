#pragma once
#include <stddef.h>

#define SUCCESS 0
#define INVALID_PARAMETERS -1
#define UNKNOWN_ERROR 1
#define LACK_OF_MEMORY -2
/************************************************************************
		������������ ������ �1

	�������� ������������ ������� ������ ��������� ������

 ************************************************************************/



typedef char* VA;				// ��� ����������� ����� ����� 

typedef struct MemoryBlock {	// ��� ����������� ���� ������
	VA va;
	size_t size;
	size_t startByte;
	struct MemoryBlock* next;
	struct MemoryBlock* previous;
} MemoryBlock;

typedef struct List {			// ��� ����������� ��� ��������� ������
	size_t size_memory;
	size_t size_free_memory;
	MemoryBlock* head;
	MemoryBlock* last;
} List;

List* list;

/**
	@func	_malloc
	@brief	�������� ���� ������ ������������� �������

	@param	[out] ptr		������ �����
	@param	[in]  szBlock	������ �����

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	�������� ������
	@retval	1	����������� ������
 **/

int _malloc(VA* ptr, size_t szBlock);



/**
	@func	_free
	@brief	�������� ����� ������

	@param	[in] ptr		������ �����

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	1	����������� ������
 **/
int _free(VA ptr);



/**
	@func	_read
	@brief	������ ���������� �� ����� ������

	@param	[in] ptr		������ �����
	@param	[in] pBuffer	������ ������ ���� ���������� ���������
	@param	[in] szBuffer	������ ������

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	������ �� ������� �����
	@retval	1	����������� ������
 **/
int _read(VA ptr, void* pBuffer, size_t szBuffer);



/**
	@func	_write
	@brief	������ ���������� � ���� ������

	@param	[in] ptr		������ �����
	@param	[in] pBuffer	������ ������ ���� ���������� ���������
	@param	[in] szBuffer	������ ������

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	������ �� ������� �����
	@retval	1	����������� ������
 **/
int _write(VA ptr, void* pBuffer, size_t szBuffer);



/**
	@func	_init
	@brief	������������� ������ ��������� ������

	@param	[in] n		���������� �������
	@param	[in] szPage	������ ��������

	� �������� 1 � 2 ����� ����� ������ ������������� ��� n*szPage

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	1	����������� ������
 **/
int _init(int n, int szPage);

MemoryBlock* newMemoryBlock(size_t size);

MemoryBlock* findBlock(VA ptr);