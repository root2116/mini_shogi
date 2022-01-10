#ifndef TENSOR_H
#define TENSOR_H

#include <stdint.h>
#include <stdbool.h>
#include "matrix.h"

struct tensor_t
{
    int chs;
    int num;
    int rows;
    int cols;
    double *elements;
};

typedef struct tensor_t *Tensor;


Tensor create_tensor(int num, int chs, int rows, int cols);
Tensor create_tensor_at_random(int num, int chs, int rows, int cols);

void free_tensor(Tensor tensor);
double read_tensor(Tensor tensor, int n, int ch, int row, int col);
void write_tensor(double value, Tensor tensor, int n, int ch, int row, int col);
double *address_tensor(Tensor tensor, int n, int ch, int row, int col);
Matrix im2col(Tensor input_data, int filter_h, int filter_w, int stride, int pad);
Tensor pad_tensor(Tensor tensor, int pad);
Matrix reshape_to_matrix(const Tensor T, int rows, int cols);
Tensor reshape_to_tensor(const Matrix M, int num, int chs, int rows, int cols);
Tensor tensor_transpose(const Tensor M, int num, int chs, int rows, int cols);

Tensor col2im(const Matrix M, int num, int chs, int rows, int cols, int filter_h, int filter_w, int stride, int pad);

Tensor vector_reshape_to_tensor(const Vector v, int num, int chs, int rows, int cols);

Vector tensor_flatten(const Tensor T);

Tensor scalar_tensor(Tensor T, double v);
Tensor create_image_batch_tensor(double ****images, const int *batch_index, int size);
Tensor create_data_batch_tensor(Tensor X, const int *batch_index, int size);

Tensor copy_tensor(Tensor T);
#endif