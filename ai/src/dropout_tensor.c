#include "dropout_tensor.h"
#include "tensor.h"
#include "matrix.h"
#include "mask.h"

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

#include <stdio.h>
#include <stdlib.h>



static Tensor forward(DropoutTensor this, const Tensor X, bool is_backprop)
{
    if (is_backprop)
    {

        if (this->mask != NULL)
        {
            
            free_mask_tensor(this->mask);
            
        }

        
        Tensor RND = create_tensor_at_uniform_random(X->num, X->chs, X->rows, X->cols);

        Tensor M = create_tensor(X->num, X->chs, X->rows, X->cols);
        this->mask = create_mask_tensor(X->num, X->chs, X->rows, X->cols);



        for (int i = 0; i < M->num; ++i){
            for (int j = 0; j < M->chs; ++j){
                for(int k = 0; k < M->rows; k++){
                    for(int l = 0; l < M->cols; l++){
                        if (read_tensor(RND, i, j, k, l) > this->dropout_ratio)
                        {
                            this->mask->elements[i][j][k][l] = true;
                            double value = read_tensor(X,i,j,k,l);
                            write_tensor(value, M, i,j,k,l);

                        }
                        else
                        {
                            this->mask->elements[i][j][k][l] = false;
                            
                            write_tensor(0, M, i,j,k,l);
                        }
                    }
                }
                
            }
        }

        free_tensor(RND);

        return M;
    }
    else
    {
        Tensor M = product_scalar_tensor(X, 1.0 - this->dropout_ratio);

        return M;
    }
}

static Tensor backward(const DropoutTensor this, const Tensor X)
{
    Tensor M = create_tensor(X->num, X->chs, X->rows, X->cols);
    double value;
    for (int i = 0; i < M->num; ++i){
        for (int j = 0; j < M->chs; ++j){
            for(int k = 0; k < M->rows; k++){
                for(int l = 0; l < M->cols; l++){
                    if (this->mask->elements[i][j][k][j])
                    {
                        value = read_tensor(X,i,j,k,l);
                        write_tensor(value,M, i,j,k,l);
                        

                    }
                    else
                    {
                        write_tensor(0, M, i,j ,k,l);
                    }
                }
            }
            
        }
    }

    return M;
}

DropoutTensor new_dropout_tensor(double dropout_ratio)
{
    DropoutTensor d = malloc(sizeof(*d));
    d->dropout_ratio = dropout_ratio;
    d->mask = NULL;

    d->forward = forward;
    d->backward = backward;
    
    return d;
}
