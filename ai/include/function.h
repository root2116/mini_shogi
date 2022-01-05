#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdint.h>
#include "matrix.h"

Vector vector_sigmoid(const Vector v);
Matrix matrix_sigmoid(const Matrix M);
Vector vector_softmax(const Vector v);
Matrix matrix_softmax(const Matrix M);
double mean_squared_error(Vector y, Vector t);
double cross_entropy_error(Vector y, Vector t);
double batch_cross_entropy_error(Matrix y, Matrix t);
double numerical_diff(double (*f)(), double x);
Vector gradient_descent(double (*f)(), Vector init_x, double lr, int step_num);
double function_1(double x);
double function_2(Vector x);
double Uniform();
double normal_distribution_random_value(double mu, double sigma);

#endif