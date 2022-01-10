#include "affine.h"
#include "matrix.h"
#include "util.h"

#define LEAK_DETECT
#ifdef LEAK_DETECT
#include "leakdetect.h"
#define init leak_detect_init
#define malloc(s) leak_detelc_malloc(s, __FILE__, __LINE__)
#define free leak_detect_free
#define check leak_detect_check
#else
#define init()
#define check()
#endif


#include <stdlib.h>

static Matrix forward(Affine this, Matrix x, bool is_backprop){
    if(is_backprop){
        if(this->x != NULL){
            free_matrix(this->x);
        }
        this->x = copy_matrix(x);
    }
    



    Matrix t = dot_matrix(x, this->W);
    Matrix out = add_vector_matrix(this->b, t);

    free_matrix(t);

    return out;
}

static Matrix backward(Affine this, Matrix dout){
    
    Matrix W_T = transpose(this->W);
    Matrix dx = dot_matrix(dout, W_T);
    free_matrix(W_T);

    

    Matrix x_T = transpose(this->x);
    if(this->dW != NULL){
        free_matrix(this->dW);
    }
    this->dW = dot_matrix(x_T, dout);

    if(this->db != NULL){
        free_vector(this->db);
    }
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