#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>しているので、スタックの構造体はここで仮定します
#include <stdbool.h>


// STACK 構造体の仮定
typedef struct {
    void* stack_pointer;    // スタックポインタ
    void* stack_memory;     // スタックメモリの先頭
    void* end;              // スタックメモリの末尾
} STACK;


// mem_size の容量でスタック用のメモリを確保する
void initialize(STACK* s, size_t mem_size)
{
    if (s == NULL) return;

    // メモリを確保して、スタックの初期設定を行う
    s->stack_memory = malloc(mem_size);  // メモリを確保
    if (s->stack_memory == NULL) return;  // メモリ確保失敗

    s->stack_pointer = s->stack_memory;  // スタックポインタはメモリの先頭
    s->end = (char*)s->stack_memory + mem_size;  // メモリの末尾を指す
}

// 確保したメモリを解放する
void finalize(STACK* s)
{
    if (s == NULL || s->stack_memory == NULL) return;  // メモリが未確保なら何もしない

    free(s->stack_memory);  // メモリを解放
    s->stack_memory = NULL;  // 解放後はNULLに設定
    s->stack_pointer = NULL; // スタックポインタもNULLに設定
    s->end = NULL;           // 末尾ポインタもNULLに設定
}

// valの値をスタックに積む。実行の成否を返す
bool push(STACK* s, int val)
{
    if (s == NULL || s->stack_pointer == NULL || s->stack_pointer >= s->end) {
        return false;  // スタックがいっぱいまたはメモリがない場合は失敗
    }

    // 値をスタックに保存
    *(int*)s->stack_pointer = val;
    s->stack_pointer = (char*)s->stack_pointer + sizeof(int);  // スタックポインタを更新

    return true;  // 成功
}

// addrから始まるnum個の整数をスタックに積む。実行の成否を返す
bool push_array(STACK* s, int* addr, int num)
{
    if (s == NULL || s->stack_pointer == NULL || addr == NULL) {
        return false;  // 引数が無効な場合は失敗
    }

    for (int i = 0; i < num; i++) {
        if (s->stack_pointer >= s->end) {
            return false;  // スタックがいっぱいになった場合
        }
        *(int*)s->stack_pointer = addr[i];  // 配列の値をスタックに保存
        s->stack_pointer = (char*)s->stack_pointer + sizeof(int);  // スタックポインタを更新
    }

    return true;  // 成功
}

// スタックから一つの要素を取り出す
int pop(STACK* s)
{
    if (s == NULL || s->stack_pointer == s->stack_memory) {
        return 0;  // スタックが空または無効な場合は0を返す
    }

    // スタックポインタを1つ戻して、値を取り出す
    s->stack_pointer = (char*)s->stack_pointer - sizeof(int);
    int val = *(int*)s->stack_pointer;  // 値を取り出す

    return val;
}

// addrにスタックからnumの要素を取り出す。取り出せた個数を返す
int pop_array(STACK* s, int* addr, int num)
{
    if (s == NULL || s->stack_pointer == s->stack_memory || addr == NULL) {
        return 0;  // 無効な引数の場合は0を返す
    }

    int count = 0;
    for (int i = 0; i < num; i++) {
        if (s->stack_pointer == s->stack_memory) {
            break;  // スタックが空になった場合
        }

        s->stack_pointer = (char*)s->stack_pointer - sizeof(int);  // スタックポインタを1つ戻す
        addr[i] = *(int*)s->stack_pointer;  // 値を取り出して配列に保存
        count++;
    }

    return count;  // 実際に取り出した個数を返す
}
