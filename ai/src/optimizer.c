#include "optimizer.h"
#include <stdlib.h>



Optimizer new_optimizer(double lr){
    Optimizer instance = malloc(sizeof(*instance));

    instance->lr = lr;
    instance->iter = 0;

    instance->beta1 = 0;
    instance->beta2 = 0;
    instance->m = NULL;
    instance->v = NULL;

    return instance;
}