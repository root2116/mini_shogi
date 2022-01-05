#ifndef AFFINE_TENSOR_H
#define AFFINE_TENSOR_H
#include <stdbool.h>

#include "matrix.h"
#include "tensor.h"

typedef struct affine_tensor_t *AffineTensor;

struct affine_tensor_t
{
    Matrix W;
    Vector b;
    Matrix X;
    Matrix dW;
    Vector db;
    int original_num;
    int original_chs;
    int original_rows;
    int original_cols;
    Matrix (*forward)(AffineTensor, Tensor);
    Tensor (*backward)(AffineTensor, Matrix);
};

AffineTensor new_affine_tensor(Matrix W, Vector b);
#endif