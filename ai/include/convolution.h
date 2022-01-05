#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <stdint.h>
#include <stdbool.h>

#include "matrix.h"
#include "tensor.h"


typedef struct convolution_t *Convolution;

struct convolution_t{
    Tensor W;
    Vector b;
    int stride;
    int pad;
    Tensor x;
    Matrix col;
    Matrix col_W;
    Tensor dW;
    Vector db;
    Tensor (*forward)(Convolution, Tensor);
    Tensor (*backward)(Convolution, Tensor);
};

Convolution new_convolution(Tensor, Vector, int, int);
#endif