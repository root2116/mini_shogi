#ifndef RELU_TENSOR_H
#define RELU_TENSOR_H
#include <stdbool.h>

struct vector_t;
typedef struct vector_t *Vector;

struct matrix_t;
typedef struct matrix_t *Matrix;


struct tensor_t;

typedef struct tensor_t *Tensor;

typedef struct mask_tensor_t *MaskTensor;
struct mask_tensor_t
{
    int num;
    int chs;
    int rows;
    int cols;
    bool ****elements;
};

typedef struct relu_tensor_t *ReluTensor;

struct relu_tensor_t
{
    MaskTensor mask;
    Tensor (*forward)(ReluTensor, Tensor);
    Tensor (*backward)(ReluTensor, Tensor);
};



ReluTensor new_relu_tensor();
#endif