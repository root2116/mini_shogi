
#ifndef DROPOUT_TENSOR_H
#define DROPOUT_TENSOR_H
#include <stdint.h>
#include <stdbool.h>
#include "mask.h"
#include "tensor.h"

typedef struct dropout_tensor_t *DropoutTensor;
struct dropout_tensor_t
{
    double dropout_ratio;
    MaskTensor mask;
    Tensor (*forward)(DropoutTensor, Tensor, bool);
    Tensor (*backward)(DropoutTensor, Tensor);

};

DropoutTensor new_dropout_tensor(double dropout_ratio);

#endif