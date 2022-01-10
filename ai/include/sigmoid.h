#ifndef SIGMOID_H
#define SIGMOID_H
#include <stdbool.h>

#include "matrix.h"


typedef struct sigmoid_t *Sigmoid;

struct sigmoid_t
{
    Matrix y;
    Vector t;
    double loss;
    double (*forward)(Sigmoid, Matrix, Vector, bool);
    Matrix (*backward)(Sigmoid, double);
};

Sigmoid new_sigmoid();
#endif