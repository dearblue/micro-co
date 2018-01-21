/*
 * This code is under public domain (CC0)
 * <https://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * To the extent possible under law, dearblue has waived all copyright
 * and related or neighboring rights to this work.
 *
 *     dearblue <dearblue@users.noreply.github.com>
 */

#include <micro-co.h>
#include <stdio.h>
#include <stdlib.h>

static int
test_child(co_state_t *co)
{
    static int i;

    co_begin(co);

    for (i = 1; i < 5; i ++) {
        co_yield(i * 10);
    }

    co_end();

    return -1;
}

static int
test_top(co_state_t *co, co_state_t *child)
{
    static int i = 0;
    int t;

    co_begin(co);

    for (; i < 2; i ++) {
        co_yield(i);
    }

    for (; i < 5; i ++) {
        *child = CO_INIT;

        for (;;) {
            t = test_child(child);
            if (t < 0) {
                break;
            }

            co_yield(i + t);
        }
    }

    for (; i < 8; i ++) {
        co_yield(i);
    }

    co_end();

    return -1;
}

int
main(int argc, char *argv[])
{
    co_state_t co = CO_INIT, child = CO_INIT;

    int i = 0, n;
    while ((n = test_top(&co, &child)) >= 0) {
        printf("%3d: %3d\n", i ++, n);
    }

    return 0;
}
