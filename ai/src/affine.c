#include "affine.h"
#include "matrix.h"
#include "util.h"
#include <stdlib.h>

static Matrix forward(Affine this, Matrix x){
    this->x = copy_matrix(x);
    


    Matrix t = dot_matrix(x, this->W);
    Matrix out = add_vector_matrix(this->b, t);

    return out;
}

static Matrix backward(Affine this, Matrix dout){
    
    Matrix W_T = transpose(this->W);
    Matrix dx = dot_matrix(dout, W_T);
    free_matrix(W_T);

    

    Matrix x_T = transpose(this->x);
    this->dW = dot_matrix(x_T, dout);
    this->db = sum_value_in_cols(dout);

    free_matrix(x_T);

    return dx;
}

Affine new_affine(Matrix W, Vector b){
    Affine instance = malloc(sizeof(*instance));

    instance->W = W;
    instance->b = b;
    instance->x = NULL;
    instance->dW = NULL;
    instance->db = NULL;

    instance->forward = forward;
    instance->backward = backward;

    return instance;
}