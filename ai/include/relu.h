#ifndef RELU_H
#define RELU_H
#include <stdbool.h>

#include "matrix.h"

typedef struct relu_t *Relu;

struct relu_t
{
    bool *mask;
    Matrix (*forward)(Relu, Matrix);
    Matrix (*backward)(Relu, Matrix);

};

Relu new_relu();
#endif