#include "relu_tensor.h"
#include "mask.h"
#include "tensor.h"

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


static Tensor forward(ReluTensor R, const Tensor x, bool is_backprop) {

    if(is_backprop){
        if (R->mask != NULL)
        {
            free_mask_tensor(R->mask);
        }

        R->mask = create_mask_tensor(x->num, x->chs, x->rows, x->cols);
    }
   

    Tensor T = create_tensor(x->num, x->chs, x->rows, x->cols);

    for (int i = 0; i < T->num; ++i) {
        for (int j = 0; j < T->chs; ++j) {
            for (int k = 0; k < T->rows; ++k) {
                for (int l = 0; l < T->cols; ++l) {
                    if (read_tensor(x,i,j,k,l)<= 0) {
                        if(is_backprop){
                            R->mask->elements[i][j][k][l] = true;
                        }   
                        
                        write_tensor(0,T,i,j,k,l);
                        
                    } else {

                        if(is_backprop){
                            R->mask->elements[i][j][k][l] = false;
                        }
                        
                        double value = read_tensor(x,i,j,k,l);
                        write_tensor(value, T,i,j,k,l);
                        
                    }
                }
            }
        }
    }

    return T;
}

static Tensor backward(ReluTensor R, const Tensor D) {
    Tensor M = create_tensor(D->num, D->chs, D->rows, D->cols);
    for (int i = 0; i < M->num; ++i) {
        for (int j = 0; j < M->chs; ++j) {
            for (int k = 0; k < M->rows; ++k) {
                for (int l = 0; l < M->cols; ++l) {
                    if (R->mask->elements[i][j][k][l]) {

                        write_tensor(0,M,i,j,k,l);
                    } else {
                        double value = read_tensor(D,i,j,k,l);
                        write_tensor(value,M,i,j,k,l);
                    }
                }
            }
        }
    }

    return M;
}



ReluTensor new_relu_tensor(){
    ReluTensor instance = malloc(sizeof(*instance));

    instance->mask = NULL;

    instance->forward = forward;
    instance->backward = backward;
    return instance;

}

