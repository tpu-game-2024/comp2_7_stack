#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// mem_size の容量でスタック用のメモリを確保する
void initialize(STACK* s, size_t mem_size)
{
	if (s == NULL) return;

	// ToDo: mem_sizeでメモリを確保しよう

	s->stack_memory = malloc(mem_size);
	if (s->stack_memory == NULL) {
		s->stack_pointer = NULL;
		s->end = NULL;
		return;
	}

	s->end = (int*)&s->stack_memory[mem_size / sizeof(int)];
	s->stack_pointer = s->end;
}


// 確保したメモリを解放する
void finalize(STACK* s)
{
	// ToDo: Initializeで確保したメモリを解放しよう

	if (s == NULL) 
		return;

	free(s->stack_memory);
	s->stack_memory = NULL;
	s->stack_pointer = NULL;
	s->end = NULL;
}


// valの値をスタックに積む。実行の成否を返す
bool push(STACK* s, int val)
{
	// ToDo: valの値をスタックに保存しよう

	if (s == NULL || s->stack_memory == NULL)
		return false;

	if (s->stack_pointer <= s->stack_memory)
		return false;

	s->stack_pointer--;
	*(s->stack_pointer) = val;

	return true;
}


// addrから始まるnum個の整数をスタックに積む。実行の成否を返す
bool push_array(STACK* s, int* addr, int num)
{
	// ToDo: addrからはじまるnum個の整数をスタックに保存しよう

	if (s == NULL || s->stack_memory == NULL || addr == NULL || num <= 0)
		return false;

	if (s->stack_pointer < s->stack_memory + num) 
		return false;

	s->stack_pointer -= num;
	memcpy(s->stack_pointer, addr, num * sizeof(int));

	return true;
}

// スタックから一つの要素を取り出す
int pop(STACK* s)

{
	if (s == NULL || s->stack_memory == NULL || s->stack_pointer == s->end)
		return 0; // エラー時は0を返す

	int s_val = *(s->stack_pointer);
	s->stack_pointer++; // スタックポインタを進める
	return s_val;
}

// addrにスタックからnumの要素を取り出す。取り出せた個数を返す
int pop_array(STACK* s, int* addr, int num)
{
	// ToDo: スタックからnum個の値を取り出してaddrから始まるメモリに保存しよう
	// スタックにnum個の要素がたまっていなかったら、積まれている要素を返して、
	// 積んだ要素数を返り値として返そう

	if (s == NULL || s->stack_memory == NULL || addr == NULL || num <= 0)
		return 0;

	int remaining_elements = (int)(s->end - s->stack_pointer);

	int elements_to_pop = (remaining_elements < num) ? remaining_elements : num;

	memcpy(addr, s->stack_pointer, elements_to_pop * sizeof(int));
	s->stack_pointer += elements_to_pop;

	return elements_to_pop;
}