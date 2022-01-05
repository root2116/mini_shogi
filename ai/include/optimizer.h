#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "tensor.h"
#include <stdint.h>
#include <stdbool.h>

enum{
    SGD,
    Momentum,
    AdaGrad,
    Adam
};



typedef struct optimizer_t *Optimizer;

struct params_t
{
    Tensor W[6];
    Matrix W1;
    Matrix W2;
    Vector b[6];
    Vector b1;
    Vector b2;
};

typedef struct params_t *Params;

struct optimizer_t{
    double lr;
    double beta1;
    double beta2;
    int iter;
    Params m;
    Params v;
    void (*update)(Optimizer, Params, Params);
};


Optimizer new_optimizer(double lr);
#endif