#ifndef ADAM_H
#define ADAM_H

#include <stdint.h>
#include <stdbool.h>

#include "optimizer.h"



Optimizer new_adam(double lr, double beta1, double beta2);
#endif