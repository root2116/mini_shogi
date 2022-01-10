#include <stdio.h>
#include <stdlib.h>
#include "leakdetect.h"

/* 管理するメモリの情報の最大数 */
#define N 1500

/* メモリ情報を格納する構造体の配列 */
MEM_T mem_info[N];

/* メモリ管理配列を初期化する関数 */
void leak_detect_init(void)
{
    int i = 0;

    /* 各メモリの情報を初期化 */
    for (i = 0; i < N; i++)
    {
        mem_info[i].ptr = NULL;
        mem_info[i].size = 0;
        mem_info[i].file = NULL;
        mem_info[i].line = 0;
    }
}

/* メモリ確保とそのメモリの情報を記録する関数 */
void *leak_detelc_malloc(size_t size, const char *file, unsigned int line)
{
    int i = 0;
    void *ptr = NULL;

    /* まずはメモリを確保 */
    ptr = malloc(size);

    /* メモリ確保に失敗した場合はメモリ管理構造体には情報を格納しない */
    if (ptr == NULL)
    {
        return NULL;
    }

    for (i = 0; i < N; i++)
    {
        /* 情報が格納されていない要素にメモリの情報を格納 */
        if (mem_info[i].ptr == NULL)
        {
            mem_info[i].ptr = ptr;
            mem_info[i].size = size;
            mem_info[i].file = file;
            mem_info[i].line = line;
            break;
        }
    }

    /* 確保したメモリのアドレスを返却 */
    return ptr;
}

/* メモリの確保とそのアドレスのメモリ情報を記録から削除する関数 */
void leak_detect_free(void *ptr)
{
    int i = 0;

    for (i = 0; i < N; i++)
    {
        /* free しようとしているアドレスのメモリ構造体を探す */
        if (mem_info[i].ptr == ptr)
        {
            /* そのメモリはちゃんと解放されるのでその構造体の情報を消す */
            mem_info[i].ptr = NULL;
            mem_info[i].size = 0;
            mem_info[i].file = NULL;
            mem_info[i].line = 0;
            break;
        }
    }

    /* メモリを解放 */
    free(ptr);
}

/* 解放が行われていないメモリの情報を表示する関数 */
void leak_detect_check(void)
{
    int i = 0;
    int count = 0;
    for (i = 0; i < N; i++)
    {
        /* 情報が消されていないメモリ管理構造体を探す */
        if (mem_info[i].ptr != NULL)
        {
            /* 情報が消されていない構造体の ptr が指すメモリは解放忘れ */

            /* その解放忘れのメモリの情報を出力 */
            // printf("メモリリークを検出!!!!!\n");
            // printf(" アドレス:%p\n", mem_info[i].ptr);
            // printf(" サイズ:%u\n", (unsigned int)mem_info[i].size);
            // printf(" 場所:%s:%u\n", mem_info[i].file, mem_info[i].line);
            // printf("\n");
            count++;
        }
    }

    printf("総メモリリーク数: %d\n",count);

}