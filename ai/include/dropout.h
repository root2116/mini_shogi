
#ifndef DROPOUT_H
#define DROPOUT_H
#include <stdint.h>
#include <stdbool.h>
#include "mask.h"
#include "matrix.h"
typedef struct dropout_t *Dropout;
struct dropout_t
{
    double dropout_ratio;
    Mask mask;
    Matrix (*forward)(Dropout, Matrix, bool);
    Matrix (*backward)(Dropout, Matrix);
};

Dropout new_dropout(double dropout_ratio);

#endif