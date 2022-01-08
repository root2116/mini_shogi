#include "sigmoid.h"
#include "matrix.h"
#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double forward(Sigmoid this, Matrix x, Vector t){
    this->t = t;
    double delta = 1e-9;
    this->y = matrix_sigmoid(x);

    
    int batch_size = t->size;
    double loss = 0;
    for(int i = 0; i < this->y->rows; i++){
        
        loss += -( t->elements[i] * log(this->y->elements[i] + delta) + (1 - t->elements[i]) * log(1 - this->y->elements[i] + delta));
    }

    this->loss =  loss/batch_size;
    if(this->loss < 0 ){
        printf("loss is negative!\n");
        save_matrix("x.txt",x);
        save_matrix("y.txt", this->y);
        save_vector("t.txt",t);
    }


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