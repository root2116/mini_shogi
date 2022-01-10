#include "sigmoid.h"
#include "matrix.h"
#include "function.h"

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
#include <math.h>

double forward(Sigmoid this, Matrix x, Vector t, bool is_backprop){
    
    Matrix y = matrix_sigmoid(x);

    if (is_backprop)
    {
        if(this->t != NULL){
            free_vector(this->t);
        }
        this->t = copy_vector(t);

        if(this->y != NULL){
            free_matrix(this->y);
        }
        this->y = copy_matrix(y);
    }

    double delta = 1e-9;
    

    
    int batch_size = t->size;
    double loss = 0;
    for(int i = 0; i < y->rows; i++){
        
        loss += -( t->elements[i] * log(y->elements[i] + delta) + (1 - t->elements[i]) * log(1 - y->elements[i] + delta));
    }

    this->loss =  loss/batch_size;


    if(this->loss < 0 ){
        printf("loss is negative!\n");
        save_matrix("x.txt",x);
        save_matrix("y.txt", this->y);
        save_vector("t.txt",t);
    }

    free_matrix(y);

    return this->loss;
    
}

Matrix backward(Sigmoid this, double dout){
    
    Matrix dx = create_matrix(this->y->rows, this->y->cols);


    for(int i = 0; i < dx->rows; i++){
        dx->elements[i] = (this->y->elements[i] - this->t->elements[i]) / this->t->size;
    }

    return dx;
}

Sigmoid new_sigmoid(){
    Sigmoid instance = malloc(sizeof(*instance));

    instance->y = NULL;
    instance->t = NULL;
    instance->forward = forward;
    instance->backward = backward;

    return instance;
}