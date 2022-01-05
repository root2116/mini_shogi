#include "matrix.h"
#include "util.h"
#include <stdio.h>
#include <math.h>

#include <stdint.h>

Vector vector_sigmoid(const Vector v)
{
    Vector result = create_vector(v->size);

    for (int i = 0; i < v->size; i++)
    {
        result->elements[i] = 1 / (1 + exp(-v->elements[i]));
    }

    return result;
}

//テスト済み
Matrix matrix_sigmoid(const Matrix M){
    Matrix result = create_matrix(M->rows, M->cols);

    for(int i = 0; i < M->rows; i++){
        for(int j = 0; j < M->cols; j++){
            result->elements[i * result->cols + j] = 1 / (1 + exp(-M->elements[i * M->cols + j]));
        }
    }

    return result;
}


Vector vector_softmax(const Vector v)
{
    double c = max_value_in_vector(v);
    Vector exp_a = create_vector(v->size);

    for (int i = 0; i < v->size; i++)
    {
        exp_a->elements[i] = exp(v->elements[i] - c);
    }

    double sum_exp_a = sum_values_in_vector(exp_a);
    Vector y = create_vector(v->size);

    for (int i = 0; i < v->size; i++)
    {
        y->elements[i] = exp_a->elements[i] / sum_exp_a;
    }

    free_vector(exp_a);

    return y;
}

//テスト済み
Matrix matrix_softmax(const Matrix M){
    Matrix result = create_matrix(M->rows, M->cols);

    for(int i = 0; i < M->rows; i++){
        double c = max_value_in_row(M,i);

        Vector exp_a = create_vector(M->cols);

        for (int j = 0; j < M->cols; j++){
            exp_a->elements[j] = exp(M->elements[i * M->cols + j] - c);
        }
        
        double sum_exp_a = sum_values_in_vector(exp_a);

        for(int j = 0; j < M->cols; j++){
            result->elements[i * result->cols + j] = exp_a->elements[j] / sum_exp_a;
        }

        free_vector(exp_a);
    }

    return result;
}

Vector identity_vector(const Vector v)
{
    return v;
}

Matrix identity_matrix(const Matrix M)
{
    return M;
}


double mean_squared_error(Vector y, Vector t){

    if(y->size != t->size){
        fprintf(stderr, "Invalid size.(mean_squared_error)\n");
        return NAN;
    }
    Vector squared = create_vector(y->size);
    
    for(int i = 0; i < y->size; i++){
        double sq = y->elements[i] - t->elements[i];
        squared->elements[i] = sq * sq;
    }

    return sum_values_in_vector(squared) * 0.5;

}


double cross_entropy_error(Vector y, Vector t){
    if(y->size != t->size){
        fprintf(stderr, "Invalid size.(cross_entropy_error)\n");
        return NAN;
    }
    double delta = 1e-7;
    Vector r = create_vector(y->size);
    
    for(int i = 0; i < y->size; i++){
        r->elements[i] = t->elements[i] * log(y->elements[i] + delta); 
    }

    return -sum_values_in_vector(r);
}

//テスト済み
double batch_cross_entropy_error(Matrix y, Matrix t){
    

    double delta = 1e-7;
    Vector r = create_vector(y->rows);

    int batch_size = y->rows;
    for(int i = 0; i < y->rows; i++){
        for(int j = 0; j < y->cols; j++){
            r->elements[i] += t->elements[i * t->cols + j] * log(y->elements[i * y->cols + j] + delta);
        }
    }

    return -sum_values_in_vector(r)/batch_size;

}

double numerical_diff(double (*f)(), double x){
    double h = 1e-4;
    
    return (f(x + h) - f(x - h)) / (2 * h);
}

// Vector numerical_gradient(double (*f)(), Vector x){
//     double h = 1e-4;
//     Vector grad = create_vector(x->size);

//     for(int i = 0; i < x->size; i++){
//         double tmp_val = x->elements[i];
//         x->elements[i] = tmp_val + h;
//         double fxh1 = f(x);

//         x->elements[i] = tmp_val - h;
//         double fxh2 = f(x);

//         grad->elements[i] = (fxh1 - fxh2) / (2*h);
//         x->elements[i] = tmp_val;
//     }

//     return grad;
// }

// Vector gradient_descent(double (*f)(), Vector init_x, double lr, int step_num){
//     Vector x = init_x;

//     for(int i = 0; i < step_num; i++){
//         Vector grad = numerical_gradient(f,x);
//         Vector tmp = product_scalar_vector(-lr,grad);
//         x = add_vector(x, tmp);
//         free_vector(tmp);
//         free_vector(grad);
//     }

//     return x;
// }

double function_1(double x){
    return (0.01*x*x) + 0.1*x;
}


double function_2(Vector x){
    return x->elements[0] * x->elements[0] + x->elements[1] * x->elements[1];
}

