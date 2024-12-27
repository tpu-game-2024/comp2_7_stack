#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// mem_size の容量でスタック用のメモリを確保する
void initialize(STACK* s, size_t mem_size)
{
	// ToDo: mem_sizeでメモリを確保しよう
	if (s == NULL) return;
	
	s->stack_memory =  (int*)malloc(mem_size);
	if (s->stack_memory == NULL)
	{
		s->stack_pointer = NULL;
		s->end = NULL;
		return;
	}
	s->end = &s->stack_memory[mem_size / sizeof(int)];
	s->stack_pointer = s->end;
}


// 確保したメモリを解放する
void finalize(STACK* s)
{
	if (s == NULL)return;
	free(s->stack_memory);
	s->stack_memory = NULL;
	s->stack_pointer = NULL;
	s->end = NULL;
}


// valの値をスタックに積む。実行の成否を返す
bool push(STACK* s, int val)
{
	if (s == NULL)return false;
	if (s->stack_pointer < s->stack_memory + 1) {
		return false;
	}
	else {
		s->stack_pointer--;
		*s->stack_pointer = val;
		return true;
	}
}


// addrから始まるnum個の整数をスタックに積む。実行の成否を返す
bool push_array(STACK* s, int* addr, int num)
{
	if (s == NULL)return false;

	if (addr == NULL || num <= 0) return false;

	if (s->stack_memory < num)    return false;

	if (s->stack_pointer < s->stack_memory + num) return false;
	
	for (int i = num - 1; i >= 0; i--)
	{
		push(s, addr[i]);
	}

	return true;
}

// スタックから一つの要素を取り出す
int pop(STACK* s)
{
	// ToDo: スタックの最上位の値を取り出して返そう
	if (s == NULL) return 0;

	if (s->end < s->stack_pointer + 1)return 0;

	
		int num = *s->stack_pointer;
		s->stack_pointer++;
		return num;
}

// addrにスタックからnumの要素を取り出す。取り出せた個数を返す
int pop_array(STACK* s, int* addr, int num)
{
	// ToDo: スタックからnum個の値を取り出してaddrから始まるメモリに保存しよう
	// スタックにnum個の要素がたまっていなかったら、積まれている要素を返して、
	// 積んだ要素数を返り値として返そう

	if (s == NULL || s->stack_memory == NULL || s->stack_pointer == NULL || s->end == NULL)
		return 0;

	if (addr == NULL || num <= 0) return 0;

	if (s->stack_pointer == s->end) return 0;
	

	int attempts = 1;
	for (int i = 0; i < num; i++) {
		
		addr[i] = pop(s);
		if (addr[i] == 0) return i;
	}
	return num;
}