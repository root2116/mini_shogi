#include "sigmoid.h"
#include "matrix.h"
#include "function.h"
#include <stdlib.h>
#include <math.h>

double forward(Sigmoid this, Matrix x, Vector t){
    this->t = t;

    this->y = matrix_sigmoid(x);

    double loss = 0;
    for(int i = 0; i < this->y->rows; i++){
        loss += -( t->elements[i] * log(this->y->elements[i]) + (1 - t->elements[i]) * log(1 - this->y->elements[i]));
    }

    this->loss =  loss;


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