#include <tchar.h>
#include "CppUnitTest.h"

#include "../include/lib_func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(基本チェック)
	{
	public:
		
		TEST_METHOD(一要素)
		{
			STACK s;
			initialize(&s, sizeof(int));

			Assert::IsTrue(push(&s, 2), _T("2を積むのに成功する"));

			Assert::AreEqual(pop(&s), 2, _T("2を入れて2を取り出す"));

			finalize(&s);
		}

		TEST_METHOD(大量に（１０００万）積む)
		{
			STACK s;
			initialize(&s, sizeof(int)*10000000);

			for (int i = 0; i < 10000000; i++) {
				Assert::IsTrue(push(&s, i), _T("積むのに成功する"));
			}

			for (int i = 0; i < 10000000; i++) {
				Assert::AreEqual(pop(&s), 10000000-1-i, _T("上から取り出す"));
			}

			finalize(&s);
		}

		TEST_METHOD(複数要素)
		{
			STACK s;
			initialize(&s, sizeof(int)*2);

			int a[2] = { 5, 7 };
			Assert::IsTrue(push_array(&s, a, 2), _T("2要素を積むのに成功する"));

			int b[2];
			Assert::AreEqual(pop_array(&s, b, 2), 2, _T("2要素取り出す"));
			Assert::AreEqual(a[0], b[0], _T("入れた値の確認"));
			Assert::AreEqual(a[1], b[1], _T("入れた値の確認"));

			finalize(&s);
		}

		TEST_METHOD(まとめて入れてバラバラに取り出す)
		{
			STACK s;
			initialize(&s, sizeof(int) * 2);

			int a[2] = { 9, 10 };
			Assert::IsTrue(push_array(&s, a, 2), _T("2要素を積むのに成功する"));

			Assert::AreEqual(pop(&s), 9, _T("先頭から格納されることを想定"));
			Assert::AreEqual(pop(&s), 10, _T("２番目の要素を取り出す"));

			finalize(&s);
		}

		TEST_METHOD(バラバラに入れてまとめて取り出す)
		{
			STACK s;
			initialize(&s, sizeof(int) * 2);

			Assert::IsTrue(push(&s, 14), _T("14を積むのに成功する"));
			Assert::IsTrue(push(&s, 20), _T("20を積むのに成功する"));
			int b[2];
			Assert::AreEqual(pop_array(&s, b, 2), 2, _T("2要素取り出す"));
			Assert::AreEqual(b[0], 20, _T("上の要素を先頭に入れる"));
			Assert::AreEqual(b[1], 14, _T("上から２番目の要素が2番目に入る"));

			finalize(&s);
		}

		TEST_METHOD(要素を超えては詰めない)
		{
			STACK s;
			initialize(&s, sizeof(int) * 2);

			Assert::IsTrue(push(&s, 31), _T("31を積むのに成功する"));
			int a[2] = { -1, -2 };
			Assert::IsFalse(push_array(&s, a, 2), _T("2要素を積むのに失敗する"));
			Assert::IsTrue(push(&s, 37), _T("37を積むのに成功する"));
			int b[2];
			Assert::AreEqual(pop(&s), 37, _T("先頭から格納されることを想定"));
			Assert::AreEqual(pop_array(&s, b, 2), 1, _T("1要素しか残っていない"));
			Assert::AreEqual(b[0], 31, _T("1要素は正しく取り出せる"));

			finalize(&s);
		}
	};

	TEST_CLASS(品質チェック)
	{
	public:

		TEST_METHOD(NULLチェック)
		{
			initialize(NULL, 0);
			Assert::AreEqual(1, 1, _T("NULLで初期化しても破綻しない"));

			int i = 0;
			Assert::IsFalse(push(NULL, 1), _T("NULLポインタでのpushはできない"));
			Assert::IsFalse(push_array(NULL, &i, 1), _T("NULLポインタでのpush_arrayはできない"));
			Assert::AreEqual(pop(NULL), 0, _T("NULLポインタでのpopはできない"));
			Assert::IsFalse(pop_array(NULL, &i, 1), _T("NULLポインタでのpop_arrayはできない"));

			STACK s;
			initialize(&s, sizeof(int) * 1);
			Assert::IsFalse(push_array(&s, NULL, 2), _T("NULLポインタでのpushはできない"));
			Assert::AreEqual(pop_array(&s, NULL, 2), 0, _T("NULLポインタでのpopはできない"));
			Assert::IsFalse(push_array(&s, &i, 0), _T("０個のpushはできない"));
			Assert::IsFalse(push_array(&s, &i, -1), _T("負の数のpushはできない"));
			Assert::AreEqual(pop_array(&s, &i, 0), 0, _T("０個のpopはできない"));
			Assert::AreEqual(pop_array(&s, &i, -1), 0, _T("負の数のpopはできない"));

			finalize(NULL);
			Assert::AreEqual(1, 1, _T("NULLで解放しても破綻しない"));

			finalize(&s);
		}

		TEST_METHOD(ゼロサイズ確保)
		{
			STACK s;
			initialize(&s, 0);

			finalize(&s);
			Assert::AreEqual(1, 1, _T("サイズを0で指定しても問題は起きない"));
		}

		TEST_METHOD(確保した数を超えて積めない)
		{
			STACK s;
			initialize(&s, sizeof(int));

			Assert::IsTrue(push(&s, 2), _T("2を積むのに成功する"));
			Assert::IsFalse(push(&s, 3), _T("1要素分しか確保していなので積めない"));

			finalize(&s);
		}

		TEST_METHOD(取り出す要素がない時は0)
		{
			STACK s;
			initialize(&s, sizeof(int));

			Assert::IsTrue(push(&s, 2), _T("2を積むのに成功する"));

			Assert::AreEqual(pop(&s), 2, _T("2を入れて2を取り出す"));
			Assert::AreEqual(pop(&s), 0, _T("残っていないければ0を返す"));
			Assert::AreEqual(pop(&s), 0, _T("残っていないければ0を返す"));

			finalize(&s);
		}

		TEST_METHOD(取り出す要素がない時は0（複数版）)
		{
			STACK s;
			initialize(&s, sizeof(int)*2);

			Assert::IsTrue(push(&s, 2), _T("2を積むのに成功する"));
			Assert::IsTrue(push(&s, 3), _T("3を積むのに成功する"));
			Assert::AreEqual(pop(&s), 3, _T("3が一番上のはず"));
			Assert::IsTrue(push(&s, 4), _T("4を積むのに成功する"));
			Assert::AreEqual(pop(&s), 4, _T("4が一番上のはず"));
			Assert::AreEqual(pop(&s), 2, _T("2が一番上のはず"));
			Assert::AreEqual(pop(&s), 0, _T("残っていないければ0を返す"));

			finalize(&s);
		}

		TEST_METHOD(複数要素で限界を超えてつめない・出せない)
		{
			STACK s;
			initialize(&s, sizeof(int) * 2);

			int a[2] = { 5, 7 };
			Assert::IsTrue(push_array(&s, a, 2), _T("2要素を積むのに成功する"));
			Assert::IsFalse(push_array(&s, a, 2), _T("限界を超えて詰めない"));
			Assert::IsFalse(push(&s, 6), _T("限界を超えて詰めない"));

			int b[2];
			Assert::AreEqual(pop_array(&s, b, 2), 2, _T("2要素取り出す"));
			Assert::AreEqual(a[0], b[0], _T("入れた値の確認"));
			Assert::AreEqual(a[1], b[1], _T("入れた値の確認"));
			Assert::AreEqual(pop_array(&s, b, 2), 0, _T("要素がなければ取り出せない"));
			Assert::AreEqual(pop(&s), 0, _T("要素がなければ取り出せない"));

			finalize(&s);
		}

		TEST_METHOD(バラバラに入れてバラバラに取り出す)
		{
			STACK s;
			initialize(&s, sizeof(int) * 3);

			int a[2] = { 13, 17 };
			Assert::IsTrue(push_array(&s, a, 2), _T("2要素を積むのに成功する"));
			Assert::IsTrue(push(&s, 19), _T("19を積むのに成功する"));
			Assert::IsFalse(push(&s, 23), _T("23を積むのに失敗する"));

			int b[2];
			Assert::AreEqual(pop_array(&s, b, 2), 2, _T("2要素取り出す"));
			Assert::AreEqual(b[0], 19, _T("上の要素を先頭に入れる"));
			Assert::AreEqual(b[1], 13, _T("上から２番目の要素が2番目に入る"));
			Assert::AreEqual(pop(&s), 17, _T("17が一番上のはず"));
			Assert::AreEqual(pop_array(&s, b, 1), 0, _T("要素がなければ取り出せない"));
			Assert::AreEqual(b[0], 19, _T("要素は変更されていないはず"));
			Assert::AreEqual(pop(&s), 0, _T("要素がなければ取り出せない"));

			finalize(&s);
		}

		TEST_METHOD(多重解放)
		{
			STACK s;
			initialize(&s, 1);

			finalize(&s);
			finalize(&s);
			Assert::AreEqual(1, 1, _T("finalizeを2度呼んでも破綻しない"));
		}
	};
}
