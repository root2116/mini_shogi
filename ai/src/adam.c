#include "adam.h"
#include "matrix.h"
#include "tensor.h"
#include "optimizer.h"
#include "value_net.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static void update_scalar(double* x, double dx, double lr, double beta1, double beta2, double* m, double* v, int iter) {
    const double lr_t = lr * sqrt(1.0 - pow(beta2, iter + 1)) / (1.0 - pow(beta1, iter + 1)); 

    *m += (1 - beta1) * (dx - *m);
    *v += (1 - beta2) * (pow(dx, 2) - *v);

    *x -= lr_t * (*m) / (sqrt(*v) + 1e-7);
}

static void update_vector(Vector V, const Vector dV, double lr, double beta1, double beta2, Vector m, Vector v, int iter) {
    for (int i = 0; i < V->size; ++i) {
        update_scalar(&(V->elements[i]), dV->elements[i], lr, beta1, beta2, &(m->elements[i]), &(v->elements[i]), iter); 
    }
}

static void update_matrix(Matrix A, const Matrix dA, double lr, double beta1, double beta2, Matrix m, Matrix v, int iter) {
    for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < A->cols; ++j) {
            update_scalar(&(A->elements[i * A->cols + j]), dA->elements[i * dA->cols + j], lr, beta1, beta2, &(m->elements[i * m->cols + j]), &(v->elements[i * v->cols + j]), iter); 
        }
    } 
}

static void update_tensor(Tensor A, const Tensor dA, double lr, double beta1, double beta2, Tensor m, Tensor v, int iter) {
    for (int i = 0; i < A->num; ++i) {
        for (int j = 0; j < A->chs; ++j) {
            for (int k = 0; k < A->rows; ++k) {
                for (int l = 0; l < A->cols; ++l) {
                    update_scalar(address_tensor(A,i,j,k,l), read_tensor(dA,i,j,k,l), lr, beta1, beta2, address_tensor(m,i,j,k,l), address_tensor(v,i,j,k,l), iter); 


                }
            }
        }
    } 
}


static void update(Optimizer this, Params params, Params grads){

    if(this->m == NULL){
        this->m = malloc(sizeof(struct params_t));
        this->v = malloc(sizeof(struct params_t));
        

        for(int i = 0; i < CONV_DEPTH; i++){
            this->m->W[i] = create_tensor(params->W[i]->num, params->W[i]->chs, params->W[i]->rows, params->W[i]->cols);
            this->m->b[i] = create_vector(params->b[i]->size);

            this->v->W[i] = create_tensor(params->W[i]->num, params->W[i]->chs, params->W[i]->rows, params->W[i]->cols);
            this->v->b[i] = create_vector(params->b[i]->size);
        }
        this->m->W1 = create_matrix(params->W1->rows, params->W1->cols);
        this->m->W2 = create_matrix(params->W2->rows, params->W2->cols);

        this->v->W1 = create_matrix(params->W1->rows, params->W1->cols);
        this->v->W2 = create_matrix(params->W2->rows, params->W2->cols);

        this->m->b1 = create_vector(params->b1->size);
        this->m->b2 = create_vector(params->b2->size);

        this->v->b1 = create_vector(params->b1->size);
        this->v->b2 = create_vector(params->b2->size);

    }

    
    for(int i = 0; i < CONV_DEPTH; i++){
        update_tensor(params->W[i], grads->W[i], this->lr, this->beta1, this->beta2, this->m->W[i], this->v->W[i], this->iter);
        update_vector(params->b[i], grads->b[i], this->lr, this->beta1, this->beta2, this->m->b[i], this->v->b[i], this->iter);
    }
    update_matrix(params->W1, grads->W1, this->lr, this->beta1, this->beta2, this->m->W1, this->v->W1, this->iter);
    update_vector(params->b1, grads->b1, this->lr, this->beta1, this->beta2, this->m->b1, this->v->b1, this->iter);
    update_matrix(params->W2, grads->W2, this->lr, this->beta1, this->beta2, this->m->W2, this->v->W2, this->iter);
    update_vector(params->b2, grads->b2, this->lr, this->beta1, this->beta2, this->m->b2, this->v->b2, this->iter);
   

    (this->iter)++;

}


Optimizer new_adam(double lr, double beta1, double beta2){
    Optimizer new = new_optimizer(lr);
    
    new->beta1 = beta1;
    new->beta2 = beta2;

    new->update = update;

    return new;

        
}