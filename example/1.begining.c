/*
 * This code is under public domain (CC0)
 * <https://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * To the extent possible under law, dearblue has waived all copyright
 * and related or neighboring rights to this work.
 *
 *     dearblue <dearblue@users.noreply.github.com>
 */

#include <stdio.h>
#include <micro-co.h>

int
testx(co_state_t *co, int *i)
{
    /*
     * 関数の共通初期化部分。
     *
     * static 変数を置く場合はこの部分に置く。
     */



    /*
     * コルーチンの開始部分であることを示す。
     */
    co_begin(co);

    *i = 0;

    for (; *i < 15; (*i) ++) {
        if ((*i % 3) == 0) {
            co_yield(*i * 11); /* コルーチンから呼び出し元へ戻る。再開は直後から。 */
        } else if ((*i % 5) == 0) {
            co_yield(*i * 22); /* コルーチンから呼び出し元へ戻る。再開は直後から。 */
        } else {
            co_yield(-2); /* コルーチンから呼び出し元へ戻る。再開は直後から。 */
        }
    }

    *i = 100;
    co_yield(60); /* コルーチンから呼び出し元へ戻る。再開は直後から。 */

    *i = -50;
    co_yield(30); /* コルーチンから呼び出し元へ戻る。再開は直後から。 */


    /*
     * コルーチンの終了部分であることを示す。何度再開しても直後から始まる。
     */
    co_end();

    return -1;
}

int
main(int argc, char *argv[])
{
    /*
     * (REQUIRED) micro-co のコンテキスト。メモリ量は sizeof(void *) と等価。
     * 必ず CO_INIT で初期化しなければならない。
     */
    co_state_t co = CO_INIT;

    /*
     * (optional) コルーチンの内部で使う変数。
     * 存在しなくてもいいし、複数個の任意の変数や複雑な構造体を選択することも出来る。
     */
    int i;

    for (int j = 0; j < 20; j ++) {
        /*
         * micro-co コンテキストのみが必須で、あとは任意。戻り値は void でも可能。
         */
        int s = testx(&co, &i);

        printf("%d, %d, %d, %p\n", j, i, s, (void *)co);
    }

    return 0;
}
