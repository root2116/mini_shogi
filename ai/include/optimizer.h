#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "tensor.h"
#include "value_net.h"
#include <stdint.h>
#include <stdbool.h>

enum{
    SGD,
    Momentum,
    AdaGrad,
    Adam
};



typedef struct optimizer_t *Optimizer;



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