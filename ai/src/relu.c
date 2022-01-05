#include "relu.h"
#include "matrix.h"
#include <stdlib.h>


static Matrix forward(Relu this, Matrix x){

    if(this->mask != NULL){
        free(this->mask);
    }
    
    this->mask = malloc(x->rows * x->cols * sizeof(bool));


    for(int i = 0; i < x->rows; i++){
        for(int j = 0; j < x->cols; j++){
            if(x->elements[i * x->cols + j] <= 0){
                this->mask[i * x->cols + j] = true;
            }else{
                this->mask[i * x->cols + j] = false;
            }
            
        }
    }



    Matrix out = copy_matrix(x);

    for(int i = 0; i < out->rows; i++){
        for(int j = 0; j < out->cols; j++){
            if(this->mask[i * out->cols + j] == true){
                out->elements[i * out->cols + j] = 0;
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