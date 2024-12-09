#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"

//test
// mem_size の容量でスタック用のメモリを確保する
void initialize(STACK* s, size_t mem_size)
{
	if (s == NULL) return;

	// ToDo: mem_sizeでメモリを確保しよう
	s->stack_pointer = NULL;
	s->stack_memory = NULL;
	s->end = NULL;
}

// 確保したメモリを解放する
void finalize(STACK* s)
{
    if (s == NULL || s->stack_memory == NULL) return;

    free(s->stack_memory);
    s->stack_memory = NULL;
    s->stack_pointer = NULL;
    s->end = NULL;
}

// valの値をスタックに積む。実行の成否を返す
bool push(STACK* s, int val)
{
    if (s == NULL || s->stack_memory == NULL) return false;
    if (s->stack_pointer >= s->end) return false; // スタックが満杯

    *(s->stack_pointer) = val;
    s->stack_pointer++;
    return true;
}

// addrから始まるnum個の整数をスタックに積む。実行の成否を返す
bool push_array(STACK* s, int* addr, int num)
{
    if (s == NULL || s->stack_memory == NULL || addr == NULL || num <= 0) return false;
    if (s->stack_pointer + num > s->end) return false; // 十分な空きがない

    for (int i = 0; i < num; i++)
    {
        *(s->stack_pointer) = addr[i];
        s->stack_pointer++;
    }
    return true;
}

// スタックから一つの要素を取り出す
int pop(STACK* s)
{
    if (s == NULL || s->stack_memory == NULL || s->stack_pointer == s->stack_memory) return 0;

    s->stack_pointer--;
    return *(s->stack_pointer);
}

// addrにスタックからnumの要素を取り出す。取り出せた個数を返す
int pop_array(STACK* s, int* addr, int num)
{
    if (s == NULL || s->stack_memory == NULL || addr == NULL || num <= 0) return 0;

    int count = 0;
    while (count < num && s->stack_pointer > s->stack_memory)
    {
        s->stack_pointer--;
        addr[count] = *(s->stack_pointer);
        count++;
    }
    return count;
}