#include "value_net.h"
#include "data_generator.h"
#include "tensor.h"
#include "matrix.h"
#include "convolution.h"
#include "affine.h"
#include "relu.h"
#include "relu_tensor.h"
#include "affine_tensor.h"
#include "function.h"
#include "optimizer.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

void save_params(ValueNet net)
{
    Matrix W;
    Tensor T;
    char file_name[32];
    for (int i = 0; i < CONV_DEPTH; i++)
    {
        T = net->params->W[i];
        W = reshape_to_matrix(T, T->num * T->chs, T->rows * T->cols);
        sprintf(file_name, "W%d_conv.dat", i);
        save_matrix_as_binary(file_name, W);

        sprintf(file_name, "b%d_conv.dat", i);
        save_vector_as_binary(file_name, net->params->b[i]);
    }

    save_matrix_as_binary("W1_affine.dat", net->params->W1);
    save_matrix_as_binary("W2_affine.dat", net->params->W2);
    save_vector_as_binary("b1_affine.dat", net->params->b1);
    save_vector_as_binary("b2_affine.dat", net->params->b2);
}


void load_params(ValueNet net){

    Matrix W;
    Tensor T;
    char file_name[32];
    for(int i = 0; i < CONV_DEPTH; i++){
        sprintf(file_name, "W%d_conv.dat",i);
        Tensor tmp1 = net->params->W[i];
        Matrix tmp2 = create_matrix_from_binary_file(file_name, tmp1->num*tmp1->chs, tmp1->rows*tmp1->cols);
        net->params->W[i] = reshape_to_tensor(tmp2, tmp1->num , tmp1->chs, tmp1->rows, tmp1->cols);

        free_tensor(tmp1);


        sprintf(file_name, "b%d_conv.dat",i);
        Vector tmp3 = net->params->b[i];
        net->params->b[i] = create_vector_from_binary_file(file_name, tmp3->size);
        free_vector(tmp3);

    }

    Matrix W1 = net->params->W1;
    Matrix W2 = net->params->W2;
    Vector b1 = net->params->b1;
    Vector b2 = net->params->b2;

    net->params->W1 = create_matrix_from_binary_file("W1_affine.dat",W1->rows, W1->cols);
    net->params->W2 = create_matrix_from_binary_file("W2_affine.dat",W2->rows, W2->cols);

    net->params->b1 = create_vector_from_binary_file("b1_affine.dat",b1->size);
    net->params->b2 = create_vector_from_binary_file("b2_affine.dat",b2->size);

    free_matrix(W1);
    free_matrix(W2);
    free_vector(b1);
    free_vector(b2);


    
}

static double predict_value(ValueNet net, Game game){
    Tensor X = convert_board_into_tensor(game->board);

    Matrix R = net->predict(net, X);
    Matrix y = matrix_sigmoid(R);

    double value = y->elements[0];

    free_matrix(y);
    free_matrix(R);
    free_tensor(X);
    return value;
}

static Matrix predict(ValueNet net, Tensor x)
{
    Tensor T1 = x;
    Tensor T2;
    for(int i = 0; i < CONV_DEPTH; i++){
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

static double accuracy(ValueNet net, Tensor x, Vector t)
{
    Matrix y = net->predict(net, x);


    double sum = 0;

    for (int i = 0; i < y->rows; i++)
    {
        if(t->elements[i] == 0){
            if(y->elements[i] < 0.5){
                sum += 1;
            }
        }else{
            if(y->elements[i] > 0.5){
                sum += 1;
            }
        }
        
    }

    return sum / (double)(x->num);
}

static Params gradient(ValueNet net, Tensor X, Vector t)
{
    net->loss(net, X, t);

    Matrix T = net->layers->sigmoid_with_loss->backward(net->layers->sigmoid_with_loss, 1);

    Matrix T2 = net->layers->affine2->backward(net->layers->affine2, T);

    Matrix T3 = net->layers->relu1->backward(net->layers->relu1, T2);

    Tensor T4 = net->layers->affine1->backward(net->layers->affine1, T3);

    Tensor T5;
    for(int i = 0; i < CONV_DEPTH; i++){
        T5 = net->layers->relus[CONV_DEPTH - 1 -i]->backward(net->layers->relus[CONV_DEPTH - 1 -i], T4);
        free_tensor(T4);
        T4 = net->layers->convs[CONV_DEPTH - 1 -i]->backward(net->layers->convs[CONV_DEPTH - 1 -i], T5);
        free_tensor(T5);
    }
    

    

    free_matrix(T);
    free_matrix(T2);
    // free_matrix(T3);

    free_tensor(T4);
    // free_tensor(T5);

    Params grads = malloc(sizeof(*grads));

    for(int i = 0; i < CONV_DEPTH; i++){
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
            if(j % 5 == 0){
                printf("%d\n",j);
            }

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
    
    for(int i = 0; i < CONV_DEPTH; i++){
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

    //filter_num = 80, input_chs = 40, filter_size = 3
    Tensor tmp1 = create_tensor_at_random(filter_num, input_chs, filter_size, filter_size);
    double he1 = sqrt(2.0) / sqrt(input_chs);
    instance->params->W[0] = scalar_tensor(tmp1, he1);
    instance->params->b[0] = create_vector(filter_num);
    
    double he2 = sqrt(2) / sqrt(filter_num);
    for(int i = 1; i < CONV_DEPTH - 1; i++){
        Tensor tmp = create_tensor_at_random(filter_num, filter_num, filter_size, filter_size);
        instance->params->W[i] = scalar_tensor(tmp, he2);
        free_tensor(tmp);

        instance->params->b[i] = create_vector(filter_num);
    }

    // Tensor tmp2 = create_tensor_at_random(filter_num, 21, 1, 1);

    double he3 = sqrt(2) / sqrt(filter_num);
    Tensor tmp2 = create_tensor_at_random(21,filter_num,  1, 1);

    instance->params->W[CONV_DEPTH - 1] = scalar_tensor(tmp2, he3);

    // instance->params->b[5] = create_vector(filter_num);
    instance->params->b[CONV_DEPTH - 1] = create_vector(21);

    Matrix tmp3 = create_matrix_at_random(5*5*21,96);

    double he4 = sqrt(2) / sqrt(21);
    instance->params->W1 = product_scalar_matrix(he4, tmp3);
    instance->params->b1 = create_vector(96);

    double he5 = sqrt(2) / sqrt(96);
    Matrix tmp4 = create_matrix_at_random(96, 1);
    instance->params->W2 = product_scalar_matrix(he5, tmp4);
    instance->params->b2 = create_vector(1);

  
    instance->layers = malloc(sizeof(struct layers_t));

    
    for(int i = 0; i < CONV_DEPTH - 1; i++){
        instance->layers->convs[i] = new_convolution(instance->params->W[i], instance->params->b[i], filter_stride, filter_pad);
        instance->layers->relus[i] = new_relu_tensor();
    }

    instance->layers->convs[CONV_DEPTH - 1] = new_convolution(instance->params->W[CONV_DEPTH - 1], instance->params->b[CONV_DEPTH - 1], filter_stride, 0);
    instance->layers->relus[CONV_DEPTH - 1] = new_relu_tensor();

    instance->layers->affine1 = new_affine_tensor(instance->params->W1, instance->params->b1);
    instance->layers->relu1 = new_relu();
    instance->layers->affine2 = new_affine(instance->params->W2, instance->params->b2);

    instance->layers->sigmoid_with_loss = new_sigmoid();
    

    
    instance->predict_value = predict_value;
    instance->predict = predict;
    instance->loss = loss;
    instance->gradient = gradient;
    instance->numerical_gradient = numerical_gradient;
    instance->accuracy = accuracy;

    free_tensor(tmp1);
    free_tensor(tmp2);
    free_matrix(tmp3);
    

    return instance;
}