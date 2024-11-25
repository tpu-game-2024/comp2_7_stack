#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// STACK構造体の定義
typedef struct {
    void* stack_pointer;    // スタックポインタ
    void* stack_memory;     // スタックメモリの先頭
    void* end;              // スタックメモリの末尾
} STACK;

// mem_size の容量でスタック用のメモリを確保する
void initialize(STACK* s, size_t mem_size) {
    if (s == NULL) return;

    // メモリを確保して、スタックの初期設定を行う
    s->stack_memory = malloc(mem_size);  // メモリを確保
    if (s->stack_memory == NULL) return;  // メモリ確保失敗

    s->stack_pointer = s->stack_memory;  // スタックポインタはメモリの先頭
    s->end = (char*)s->stack_memory + mem_size;  // メモリの末尾を指す
}

// 確保したメモリを解放する
void finalize(STACK* s) {
    if (s == NULL || s->stack_memory == NULL) return;  // メモリが未確保なら何もしない

    free(s->stack_memory);  // メモリを解放
    s->stack_memory = NULL;  // 解放後はNULLに設定
    s->stack_pointer = NULL; // スタックポインタもNULLに設定
    s->end = NULL;           // 末尾ポインタもNULLに設定
}

// valの値をスタックに積む。実行の成否を返す
int push(STACK* s, int val) {
    if (s == NULL || s->stack_pointer == NULL || s->stack_pointer >= s->end) {
        return 0;  // スタックがいっぱいまたはメモリがない場合は失敗
    }

    // 値をスタックに保存
    *(int*)s->stack_pointer = val;
    s->stack_pointer = (char*)s->stack_pointer + sizeof(int);  // スタックポインタを更新

    return 1;  // 成功
}

// addrから始まるnum個の整数をスタックに積む。実行の成否を返す
int push_array(STACK* s, int* addr, int num) {
    if (s == NULL || s->stack_pointer == NULL || addr == NULL) {
        return 0;  // 引数が無効な場合は失敗
    }

    for (int i = 0; i < num; i++) {
        if (s->stack_pointer >= s->end) {
            return 0;  // スタックがいっぱいになった場合
        }
        *(int*)s->stack_pointer = addr[i];  // 配列の値をスタックに保存
        s->stack_pointer = (char*)s->stack_pointer + sizeof(int);  // スタックポインタを更新
    }

    return 1;  // 成功
}

// スタックから一つの要素を取り出す
int pop(STACK* s) {
    if (s == NULL || s->stack_pointer == s->stack_memory) {
        return 0;  // スタックが空または無効な場合は0を返す
    }

    // スタックポインタを1つ戻して、値を取り出す
    s->stack_pointer = (char*)s->stack_pointer - sizeof(int);
    int val = *(int*)s->stack_pointer;  // 値を取り出す

    return val;
}

// テスト関数
void test_push_array_pop() {
    STACK s;
    initialize(&s, sizeof(int) * 2);  // スタックに2つの整数を積むためにメモリを確保

    int a[2] = { 9, 10 };
    assert(push_array(&s, a, 2) == 1);  // 2要素をスタックに積む

    // スタックは後入れ先出し（LIFO）なので、取り出す順番は逆
    assert(pop(&s) == 10);  // 2番目に積んだ要素（10）が最初に取り出される
    assert(pop(&s) == 9);   // 1番目に積んだ要素（9）が次に取り出される

    finalize(&s);  // メモリを解放
}

int main() {
    test_push_array_pop();  // テスト実行
    printf("Test passed.\n");
    return 0;
}
