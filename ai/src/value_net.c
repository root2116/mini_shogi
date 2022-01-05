#include "value_net.h"
#include "tensor.h"
#include "matrix.h"
#include "convolution.h"
#include "affine.h"
#include "relu.h"
#include "relu_tensor.h"
#include "affine_tensor.h"
#include "function.h"
#include "optimizer.h"
#include <stdio.h>
#include <stdlib.h>

static Matrix predict(ValueNet net, Tensor x)
{
    Tensor T1 = x;
    Tensor T2;
    for(int i = 0; i < 6; i++){
        T2 = net->layers->convs[i]->forward(net->layers->convs[i], T1);
        T1 = net->layers->relus[i]->forward(net->layers->relus[i], T2);
    
        free_tensor(T2);
        
    }


    Matrix T3 = net->layers->affine1->forward(net->layers->affine1, T1);
    Matrix T4 = net->layers->relu1->forward(net->layers->relu1, T3);
    Matrix y = net->layers->affine2->forward(net->layers->affine2,T4);
    
  

    
    free_matrix(T3);
    free_matrix(T4);

    return y;
}

static double loss(ValueNet net, Tensor X, const Vector t)
{

    Matrix Y = predict(net, X);

    const double v = net->layers->sigmoid_with_loss->forward(net->layers->sigmoid_with_loss, Y, t);

    free_matrix(Y);

    return v;
}


static Params gradient(ValueNet net, Tensor X, Vector t)
{
    net->loss(net, X, t);

    Matrix T = net->layers->sigmoid_with_loss->backward(net->layers->sigmoid_with_loss, 1);

    Matrix T2 = net->layers->affine2->backward(net->layers->affine2, T);

    Matrix T3 = net->layers->relu1->backward(net->layers->relu1, T2);

    Tensor T4 = net->layers->affine1->backward(net->layers->affine1, T3);

    Tensor T5;
    for(int i = 0; i < 6; i++){
        T5 = net->layers->relus[5-i]->backward(net->layers->relus[5-i], T4);
        free_tensor(T4);
        T4 = net->layers->convs[5-i]->backward(net->layers->convs[5-i], T5);
        free_tensor(T5);
    }
    

    

    free_matrix(T);
    free_matrix(T2);
    // free_matrix(T3);

    free_tensor(T4);
    // free_tensor(T5);

    Params grads = malloc(sizeof(*grads));

    for(int i = 0; i < 6; i++){
        grads->W[i] = net->layers->convs[i]->dW;
        grads->b[i] = net->layers->convs[i]->db;
    }
    grads->W1 = net->layers->affine1->dW;
    grads->W2 = net->layers->affine2->dW;
    grads->b1 = net->layers->affine1->db;
    grads->b2 = net->layers->affine2->db;
    

    return grads;
}

static Vector vector_numerical_gradient(ValueNet net, Vector w, Tensor x, Vector t)
{
    double h = 1e-4;
    Vector grad = create_vector(w->size);

    for (int i = 0; i < w->size; i++)
    {
        double tmp_val = w->elements[i];
        w->elements[i] = tmp_val + h;
        double fxh1 = net->loss(net, x, t);

        w->elements[i] = tmp_val - h;
        double fxh2 = net->loss(net, x, t);

        grad->elements[i] = (fxh1 - fxh2) / (2 * h);
        w->elements[i] = tmp_val;
    }

    return grad;
}

static Matrix matrix_numerical_gradient(ValueNet net, Matrix M, Tensor x, Vector t)
{
    double h = 1e-4;
    Matrix grad = create_matrix(M->rows, M->cols);

    for (int i = 0; i < M->rows; i++)
    {
        if (i % 10 == 0)
        {
            printf("%d\n", i);
        }

        for (int j = 0; j < M->cols; j++)
        {

            double tmp_val = M->elements[i * M->cols + j];
            M->elements[i * M->cols + j] = tmp_val + h;
            double fxh1 = net->loss(net, x, t);

            M->elements[i * M->cols + j] = tmp_val - h;
            double fxh2 = net->loss(net, x, t);

            grad->elements[i * grad->cols + j] = (fxh1 - fxh2) / (2 * h);

            M->elements[i * M->cols + j] = tmp_val;
        }
    }

    return grad;
}

static Tensor tensor_numerical_gradient(ValueNet net, Tensor T, Tensor x, Vector t)
{
    double h = 1e-4;
    Tensor grad = create_tensor(T->num, T->chs, T->rows, T->cols);

    for (int i = 0; i < T->num; i++)
    {
        for (int j = 0; j < T->chs; j++)
        {
            for (int k = 0; k < T->rows; k++)
            {
                for (int l = 0; l < T->cols; l++)
                {
                    double tmp_val = read_tensor(T, i, j, k, l);
                    write_tensor(tmp_val + h, T, i, j, k, l);
                    double fxh1 = net->loss(net, x, t);

                    write_tensor(tmp_val - h, T, i, j, k, l);
                    double fxh2 = net->loss(net, x, t);

                    write_tensor((fxh1 - fxh2) / (2 * h), grad, i, j, k, l);

                    write_tensor(tmp_val, T, i, j, k, l);
                }
            }
        }
    }

    return grad;
}

static Params numerical_gradient(ValueNet net, Tensor x, Vector t)
{
    Params grads = malloc(sizeof(*grads));

    for(int i = 0; i < 6; i++){
        grads->W[i] = tensor_numerical_gradient(net, net->params->W[i], x, t);
        grads->b[i] = vector_numerical_gradient(net, net->params->b[i], x, t);
    }



    grads->W1 = matrix_numerical_gradient(net, net->params->W1, x, t);
    grads->b1 = vector_numerical_gradient(net, net->params->b1, x, t);
    grads->W2 = matrix_numerical_gradient(net, net->params->W2, x, t);
    grads->b2 = vector_numerical_gradient(net, net->params->b2, x, t);

    return grads;
}

ValueNet new_value_net(int input_chs, int input_rows, int input_cols, int filter_num, int filter_size, int filter_pad, int filter_stride,double weight_init_std)
{

    int input_size = input_rows;
    int conv_output_size = (input_size - filter_size + 2 * filter_pad) / filter_stride + 1;

    int pool_output_size = (int)(filter_num * (conv_output_size / 2.0) * (conv_output_size / 2.0));

    ValueNet instance = malloc(sizeof(*instance));

    instance->params = malloc(sizeof(struct params_t));

    //filter_num = 88, input_chs = 40, filter_size = 3
    Tensor tmp1 = create_tensor_at_random(filter_num, input_chs, filter_size, filter_size);
    instance->params->W[0] = scalar_tensor(tmp1, weight_init_std);
    instance->params->b[0] = create_vector(filter_num);
    

    for(int i = 1; i < 5; i++){
        Tensor tmp = create_tensor_at_random(filter_num, filter_num, filter_size, filter_size);
        instance->params->W[i] = scalar_tensor(tmp, weight_init_std);
        free_tensor(tmp);

        instance->params->b[i] = create_vector(filter_num);
    }

    // Tensor tmp2 = create_tensor_at_random(filter_num, 21, 1, 1);
    Tensor tmp2 = create_tensor_at_random(21,filter_num,  1, 1);

    instance->params->W[5] = scalar_tensor(tmp2, weight_init_std);

    // instance->params->b[5] = create_vector(filter_num);
    instance->params->b[5] = create_vector(21);

    Matrix tmp3 = create_matrix_at_random(5*5*21,96);
    instance->params->W1 = product_scalar_matrix(weight_init_std, tmp3);
    instance->params->b1 = create_vector(96);

    Matrix tmp4 = create_matrix_at_random(96, 1);
    instance->params->W2 = product_scalar_matrix(weight_init_std, tmp4);
    instance->params->b2 = create_vector(1);

  
    instance->layers = malloc(sizeof(struct layers_t));

    
    for(int i = 0; i < 5; i++){
        instance->layers->convs[i] = new_convolution(instance->params->W[i], instance->params->b[i], filter_stride, filter_pad);
        instance->layers->relus[i] = new_relu_tensor();
    }

    instance->layers->convs[5] = new_convolution(instance->params->W[5], instance->params->b[5], filter_stride, 0);
    instance->layers->relus[5] = new_relu_tensor();

    instance->layers->affine1 = new_affine_tensor(instance->params->W1, instance->params->b1);
    instance->layers->relu1 = new_relu();
    instance->layers->affine2 = new_affine(instance->params->W2, instance->params->b2);

    instance->layers->sigmoid_with_loss = new_sigmoid();
    

    

    instance->predict = predict;
    instance->loss = loss;
    instance->gradient = gradient;
    instance->numerical_gradient = numerical_gradient;
  
    free_tensor(tmp1);
    free_tensor(tmp2);
    free_matrix(tmp3);
    

    return instance;
}