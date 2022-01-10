#include "relu.h"
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


static Matrix forward(Relu this, Matrix x, bool is_backprop){

    if(is_backprop){
        if (this->mask != NULL)
        {
            free(this->mask);
        }

        this->mask = malloc(x->rows * x->cols * sizeof(bool));
    }

    Matrix out = copy_matrix(x);
    for(int i = 0; i < x->rows; i++){
        for(int j = 0; j < x->cols; j++){
            if(x->elements[i * x->cols + j] <= 0){
                out->elements[i * out->cols + j] = 0;
                
                if(is_backprop){
                    this->mask[i * x->cols + j] = true;
                }
                
            }else{
                if(is_backprop){
                    this->mask[i * x->cols + j] = false;
                }
                
            }
            
        }
    }



    

    

    return out;

}

static Matrix backward(Relu this, Matrix dout){

    for(int i = 0; i < dout->rows; i++){
        for(int j = 0; j < dout->cols; j++){
            if(this->mask[i * dout->cols + j] == true){
                dout->elements[i * dout->cols + j] = 0;
            }
        }
    }
    

    Matrix dx = dout;

    return dx;
}

Relu new_relu(){

    Relu instance = malloc(sizeof(*instance));

    instance->mask = NULL;
    instance->forward = forward;
    instance->backward = backward;

    return instance;
}