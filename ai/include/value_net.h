#ifndef INCLUDED_VALUE_NET
#define INCLUDED_VALUE_NET
#include <stdio.h>
#include "Game.h"
#include "tensor.h"

#include "convolution.h"
#include "relu.h"
#include "affine.h"
#include "relu_tensor.h"
#include "affine_tensor.h"
#include "matrix.h"
#include "tensor.h"
#include "optimizer.h"
#include "sigmoid.h"

typedef struct layers_t *Layers;

struct layers_t
{
    Convolution convs[6];
    ReluTensor relus[6];
    Relu relu1;
    AffineTensor affine1;
    Affine affine2;
    Sigmoid sigmoid_with_loss;
};

typedef struct value_net_t *ValueNet;

struct value_net_t
{
    Params params;
    Layers layers;
    Matrix (*predict)(ValueNet, Tensor);
    double (*loss)(ValueNet, Tensor x, Vector t);
    double (*accuracy)(ValueNet, Tensor x, Vector t);
    Params (*numerical_gradient)(ValueNet, Tensor x, Vector t);
    Params (*gradient)(ValueNet, Tensor x, Vector t);
};

ValueNet new_value_net(int input_chs, int input_rows, int input_cols, int filter_num, int filter_size, int filter_pad, int filter_stride,  double weight_init_std);

#endif