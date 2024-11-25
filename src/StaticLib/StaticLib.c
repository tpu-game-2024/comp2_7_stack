#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// mem_size の容量でスタック用のメモリを確保する
void initialize(STACK* s, size_t mem_size)
{
	if (s == NULL) return;

	s->stack_memory = (int*)malloc(mem_size);
	if (s->stack_memory == NULL)
	{
		s->stack_pointer = NULL;
		s->end = NULL;
	}
	else
	{
		s->stack_pointer = s->stack_memory;
		size_t num_elements = mem_size / sizeof(int);
		s->end = s->stack_memory + num_elements;
	}
}


// 確保したメモリを解放する
void finalize(STACK* s)
{
	if (s == NULL) return;

	if (s->stack_memory != NULL) {
		free(s->stack_memory);
		s->stack_memory = NULL;
		s->stack_pointer = NULL;
		s->end = NULL;
	}
}


// valの値をスタックに積む。実行の成否を返す
bool push(STACK* s, int val)
{
	if (s == NULL || s->stack_memory == NULL || s->stack_pointer == NULL || s->end == NULL)
		return false;

	if (s->stack_pointer >= s->end)
		return false;

	*(s->stack_pointer) = val;
	s->stack_pointer++;

	return true;
}


// addrから始まるnum個の整数をスタックに積む。実行の成否を返す
bool push_array(STACK* s, int* addr, int num)
{
	if (s == NULL || s->stack_memory == NULL || s->stack_pointer == NULL || s->end == NULL)
		return false;

	if (addr == NULL || num <= 0)
		return false;

	if ((s->stack_pointer + num) > s->end)
		return false;

	for (int i = num - 1; i >= 0; i--) {
		*(s->stack_pointer) = addr[i];
		s->stack_pointer++;
	}

	return true;
}

// スタックから一つの要素を取り出す
int pop(STACK* s)
{
	if (s == NULL || s->stack_memory == NULL || s->stack_pointer == NULL || s->end == NULL)
		return 0;

	if (s->stack_pointer == s->stack_memory)
		return 0;

	s->stack_pointer--;
	return *(s->stack_pointer);
}

// addrにスタックからnumの要素を取り出す。取り出せた個数を返す
int pop_array(STACK* s, int* addr, int num)
{
	if (s == NULL || s->stack_memory == NULL || s->stack_pointer == NULL || s->end == NULL)
		return 0;

	if (addr == NULL || num <= 0)
		return 0;

	int available = (int)(s->stack_pointer - s->stack_memory);
	int count = (num <= available) ? num : available;

	for (int i = 0; i < count; i++) {
		s->stack_pointer--;
		addr[i] = *(s->stack_pointer);
	}

	return count;
}