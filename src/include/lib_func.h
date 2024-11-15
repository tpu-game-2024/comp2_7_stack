#pragma once // インクルードガード

#include <stdbool.h>

// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
extern "C" {
#endif

	// 想定している構造
	//        
	// +-----+ <- stack_memory 
	// |     |
	// +-----+ 
	// |     |
	// +-----+ 
	// |     |
	// +-----+ <- stack_pointer 
	// | int |
	// +-----+ 
	// | int |
	// +-----+ 
	// | int |
	// +-----+ 
	// | int |
	// +-----+ <- end


	typedef struct
	{
		int* stack_pointer;
		int* stack_memory;
		int* end;          // スタックが底をついたか確認するためのポインタ（構造体を変更して別実装にしても良い）
	}STACK;

	void initialize(STACK* s, size_t mem_size);   // mem_size の容量でスタック用のメモリを確保する
	void finalize(STACK* s);                      // 確保したメモリを解放する

	bool push(STACK* s, int val);                 // valの値をスタックに積む。実行の成否を返す
	bool push_array(STACK* s, int* addr, int num);// addrから始まるnum個の整数をスタックに積む。実行の成否を返す

	int pop(STACK* s);                            // スタックから一つの要素を取り出す(不具合時は0を返す)
	int pop_array(STACK* s, int* addr, int num);  // addrにスタックからnumの要素を取り出す。取り出せた個数を返す


		// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
} // extern "C"
#endif
