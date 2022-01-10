#include "affine_tensor.h"
#include "tensor.h"
#include "matrix.h"

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

static Matrix forward(AffineTensor A, const Tensor X, bool is_backprop) { 
   

    A->original_num = X->num;
    A->original_chs = X->chs;
    A->original_rows = X->rows;
    A->original_cols = X->cols;

    Matrix R = reshape_to_matrix(X, X->num, -1);  

    if(is_backprop){
        if (A->X != NULL)
        {
            free_matrix(A->X);
        }

        A->X = copy_matrix(R);
    }
    

    Matrix B = dot_matrix(R, A->W);
    for (int i = 0; i < B->rows; ++i) {
        for (int j = 0; j < B->cols; ++j) {
            B->elements[i * B->cols + j] += A->b->elements[j];
        }
    }

    free_matrix(R);

    return B;
}

static Tensor backward(AffineTensor A, const Matrix D) { 

    Matrix W_T = transpose(A->W);
    Matrix X_T = transpose(A->X);

    Matrix dX = dot_matrix(D, W_T);
    if (A->dW != NULL) {
        free_matrix(A->dW);
        
    }

    A->dW = dot_matrix(X_T, D);

    if (A->db != NULL) {
        free_vector(A->db);
    }
    A->db = create_vector(D->cols);

    for (int i = 0; i < D->cols; ++i) {
        double sum = 0;
        for (int j = 0; j < D->rows; ++j) {
            sum += D->elements[j * D->cols + i];
        }
        A->db->elements[i] = sum;
    }

    // reshape
    Tensor dXR = reshape_to_tensor(dX, A->original_num, A->original_chs, A->original_rows, A->original_cols); 

    free_matrix(dX);
    free_matrix(W_T);
    free_matrix(X_T);
    return dXR;
}

AffineTensor new_affine_tensor(Matrix W, Vector b){
    AffineTensor instance = malloc(sizeof(*instance));

    instance->W = W;
    instance->b = b;
    instance->X = NULL;
    instance->dW = NULL;
    instance->db = NULL;

    instance->original_num = 0;
    instance->original_chs = 0;
    instance->original_rows = 0;
    instance->original_cols = 0;

    instance->forward = forward;
    instance->backward = backward;
    return instance;
}