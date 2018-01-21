# micro-co - 低機能で簡易な疑似コルーチンの実装

``switch`` 文のみで実装した疑似コルーチンです。

gcc (あるいは互換コンパイラ) の "label as value" が利用できるならば切り替えることが可能です。

  * Product name: [micro-co](https://github.com/dearblue/micro-co)
  * Product quality: PROTOTYPE
  * Author: [dearblue](https://github.com/dearblue)
  * Project page: <https://github.com/dearblue/micro-co>
  * Licensing: [Creative Commons Zero License (CC0 / Public Domain)](LICENSE)


## 特徴

  * (たぶん) 環境・コンパイラを選ばないで利用出来ます。
  * 引数や戻り値に制限がありません。
  * ``co_yield()`` は、入れ喰い状態になった深いブロックや ``if`` ``for`` ``while`` ``do-while`` に記述できます。
  * 省メモリ。
  * C++ でも (一応) 動作します。


### 制限

  * スケジューラはありません。全部手書きで行います。

  * ``co_XXX()`` を使っている関数のスタック変数は ``co_yield()`` する度に失われます。

  * ``co_begin()`` 〜〜〜 ``co_end()`` の中で呼び出した子関数からは ``co_yield()`` 出来ません。

    その場合は ``co_begin()`` 〜〜〜 ``co_end()`` を入れ子にして下さい。

  * ``co_begin()`` と ``co_end()`` はネストを合わせる必要があります。

  * ``co_begin()``、``co_yield()``、``co_end()`` を単一行に記述することは出来ません。

    必ず複数行に分割して記述する必要があります。

  * ``co_begin()`` 〜〜〜 ``co_end()`` の中で、``switch`` 文の中に ``co_yield()`` を含めることは出来ません。

    一つの ``co_yield()`` の中で完結した ``switch`` 文に限定されます。

      * 問題のない書き方:

        ```c
        co_begin(s);
        ...

        co_yield(0);

        switch (...) {
        case ...:
          ...;
        case ...:
          /* 入れ子にした co_begin() ... co_end() を置くことは可能 */
          ...;
        }

        co_yield(1);
        ...

        co_end();
        ```

      * ***問題のある***書き方:

        ```c
        co_begin(s);
        ...

        switch (...) {
        case ...:
          co_yield(0);
          ...;
        case ...:
          co_yield(1);
          ...;
        }

        co_end();
        ```


## 調整

  * 定義名 ``MICRO_CO_USE_SWITCH``

    コンパイラに ``MICRO_CO_USE_SWITCH`` を定義するように指定すると、内部実装を ``switch`` に切り替えます。

    gcc (あるいは互換コンパイラ) の "label as value" が利用できない環境では、常に ``MICRO_CO_USE_SWITCH`` が定義された状態となります。

    ```
    $ gcc -DMICRO_CO_USE_SWITCH ....
    ```

  * 定義名 ``MICRO_CO_FORCE_USE_LABEL_AS_VALUE``

    gcc (あるいは互換コンパイラ) の "label as value" の利用を強制します。

    ``MICRO_CO_USE_SWITCH`` を打ち消すため、可能な限り利用しないで下さい。
