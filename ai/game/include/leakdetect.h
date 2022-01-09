#ifndef LEAKDETECT_H
#define LEAKDETECT_H

#include <stdlib.h>

/* 管理数の上限 */
#define MAX_NUM 500

/* メモリ管理構造体 */
typedef struct
{
    void *ptr;
    size_t size;
    const char *file;
    unsigned int line;
} MEM_T;

/* 関数のプロトタイプ宣言 */
void leak_detect_init(void);
void *leak_detelc_malloc(size_t, const char *, unsigned int);
void leak_detect_free(void *);
void leak_detect_check(void);

#endif