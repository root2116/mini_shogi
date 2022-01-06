#include "sgd.h"
#include "optimizer.h"
#include "matrix.h"
#include "tensor.h"
#include "value_net.h"

void update_scalar(double *x, double dx, double lr)
{
    *x -= lr * dx;
}

void update_vector(Vector V, const Vector dV, double lr){
    for (int i = 0; i < V->size; ++i){
        update_scalar(&(V->elements[i]), dV->elements[i], lr);
    }
}

void update_matrix(Matrix A, const Matrix dA, double lr){
    for (int i = 0; i < A->rows; ++i){
        for (int j = 0; j < A->cols; ++j){
            update_scalar(&(A->elements[i * A->cols + j]), dA->elements[i * dA->cols + j], lr);
        }
    }
}

void update_tensor(Tensor A, const Tensor dA, double lr){
    for (int i = 0; i < A->num; ++i){
        for (int j = 0; j < A->chs; ++j){
            for (int k = 0; k < A->rows; ++k){
                for (int l = 0; l < A->cols; ++l){
            
                    update_scalar(address_tensor(A,i,j,k,l), read_tensor(A,i,j,k,l),lr);
                }
            }
        }
    }
}

static void update(Optimizer this, Params params, Params grads){

    for (int i = 0; i < CONV_DEPTH; i++)
    {
        update_tensor(params->W[i], grads->W[i], this->lr);
        update_vector(params->b[i], grads->b[i], this->lr);
    }
    update_matrix(params->W1, grads->W1, this->lr);
    update_vector(params->b1, grads->b1, this->lr);
    update_matrix(params->W2, grads->W2, this->lr);
    update_vector(params->b2, grads->b2, this->lr);

    (this->iter)++;
    
}


Optimizer new_sgd(double lr){
    Optimizer new = new_optimizer(lr);

    
    new->update= update;

    return new;
}