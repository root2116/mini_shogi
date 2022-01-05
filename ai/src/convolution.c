#include "convolution.h"
#include "tensor.h"
#include "matrix.h"


#include <stdlib.h>

static Tensor forward(Convolution this, Tensor x){
    int FN = this->W->num;
    int FH = this->W->rows;
    int FW = this->W->cols;

    int N = x->num;
    int H = x->rows;
    int W = x->cols;

    int out_h = (int)(1 + (H + 2 * this->pad - FH) / this->stride);
    int out_w = (int)(1 + (W + 2 * this->pad - FW) / this->stride);


    Matrix col = im2col(x, FH, FW, this->stride, this->pad);
    
    Matrix tmp0 = reshape_to_matrix(this->W,FN,-1);
    
    Matrix col_W = transpose(tmp0);

    Matrix tmp1 = dot_matrix(col,col_W);
    
    Matrix tmp2 = add_vector_matrix(this->b,tmp1);

    Tensor tmp3 = reshape_to_tensor(tmp2, N, out_h, out_w, -1);

    Tensor out = tensor_transpose(tmp3, 0, 3, 1, 2);


    this->x = x;
    if(this->col != NULL){
        free_matrix(this->col);
    }
    this->col = col;

    if (this->col_W != NULL)
    {
        free_matrix(this->col_W);
    }

    this->col_W = col_W;

    free_matrix(tmp0);
    free_matrix(tmp1);
    free_matrix(tmp2);
    free_tensor(tmp3);
    
    return out;

}

static Tensor backward(Convolution this, Tensor x){

    const int FN = this->W->num;
    const int C = this->W->chs;
    const int FH = this->W->rows;
    const int FW = this->W->cols;

    Tensor tmp = tensor_transpose(x, 0, 2, 3, 1);
    Matrix dout = reshape_to_matrix(tmp, -1, FN);

    // db
    if (this->db != NULL)
    {
        free_vector(this->db);
    }
    this->db = sum_value_in_cols(dout);

    

    // dW
    Matrix col_T = transpose(this->col);
    Matrix dW = dot_matrix(col_T, dout);
    Matrix dW_T = transpose(dW);

    if (this->dW != NULL)
    {
        free_tensor(this->dW);
    }

    this->dW = reshape_to_tensor(dW_T, FN, C, FH, FW);

    // dcol
    Matrix col_W_T = transpose(this->col_W);
    Matrix dcol = dot_matrix(dout, col_W_T);

    Tensor dx = col2im(dcol, this->x->num, this->x->chs, this->x->rows, this->x->cols, FH, FW, this->stride, this->pad);

    free_tensor(tmp);
    free_matrix(dout);
    free_matrix(col_T);
    free_matrix(dW);
    free_matrix(dW_T);
    free_matrix(col_W_T);
    free_matrix(dcol);

    return dx;
}

Convolution new_convolution(Tensor W, Vector b, int stride, int pad){
    Convolution instance = malloc(sizeof(*instance));

    instance->W = W;
    instance->b = b;
    instance->stride = stride;
    instance->pad = pad;

    instance->x = NULL;
    instance->col = NULL;
    instance->col_W = NULL;
    instance->db = NULL;
    instance->dW = NULL;

    instance->forward = forward;
    instance->backward = backward;

    return instance;
}