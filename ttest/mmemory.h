#pragma once
#include <stddef.h>

#define SUCCESS 0
#define INVALID_PARAMETERS -1
#define UNKNOWN_ERROR 1
#define LACK_OF_MEMORY -2
/************************************************************************
		Лабораторная работа №1

	Описание интерфейсных функций модели менеджера памяти

 ************************************************************************/



typedef char* VA;				// Тип описывающий адрес блока 

typedef struct MemoryBlock {	// Тип описывающий блок памяти
	VA va;
	size_t size;
	size_t startByte;
	struct MemoryBlock* next;
	struct MemoryBlock* previous;
} MemoryBlock;

typedef struct List {			// Тип описывающий всю доступную память
	size_t size_memory;
	size_t size_free_memory;
	MemoryBlock* head;
	MemoryBlock* last;
} List;

List* list;

/**
	@func	_malloc
	@brief	Выделяет блок памяти определенного размера

	@param	[out] ptr		адресс блока
	@param	[in]  szBlock	размер блока

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	нехватка памяти
	@retval	1	неизвестная ошибка
 **/

int _malloc(VA* ptr, size_t szBlock);



/**
	@func	_free
	@brief	Удаление блока памяти

	@param	[in] ptr		адресс блока

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
int _free(VA ptr);



/**
	@func	_read
	@brief	Чтение информации из блока памяти

	@param	[in] ptr		адресс блока
	@param	[in] pBuffer	адресс буфера куда копируется инфомация
	@param	[in] szBuffer	размер буфера

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
int _read(VA ptr, void* pBuffer, size_t szBuffer);



/**
	@func	_write
	@brief	Запись информации в блок памяти

	@param	[in] ptr		адресс блока
	@param	[in] pBuffer	адресс буфера куда копируется инфомация
	@param	[in] szBuffer	размер буфера

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
int _write(VA ptr, void* pBuffer, size_t szBuffer);



/**
	@func	_init
	@brief	Инициализация модели менеджера памяти

	@param	[in] n		количество страниц
	@param	[in] szPage	размер страницы

	В варианте 1 и 2 общий объем памяти расчитывается как n*szPage

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
int _init(int n, int szPage);

MemoryBlock* newMemoryBlock(size_t size);

MemoryBlock* findBlock(VA ptr);