#ifndef AFFINE_H
#define AFFINE_H
#include <stdbool.h>

#include "matrix.h"
#include "tensor.h"

struct affine_t;
typedef struct affine_t *Affine;

struct affine_t
{
    Matrix W;
    Vector b;
    Matrix x;
    Matrix dW;
    Vector db;
    Matrix (*forward)(Affine, Matrix, bool);
    Matrix (*backward)(Affine, Matrix);
};

Affine new_affine(Matrix W, Vector b);
#endif