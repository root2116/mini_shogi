#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdbool.h>

#define MYBLAS_PANEL_M 256

#define MYBLAS_PANEL_N 256

#define MYBLAS_PANEL_K 256

#define MYBLAS_BLOCK_M 128

#define MYBLAS_BLOCK_N 64

#define MYBLAS_BLOCK_K 128

#define MYBLAS_TILE_M 32

#define MYBLAS_TILE_N 32

#define MYBLAS_TILE_K 32

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct vector_t {
    int size;
    double* elements;
};

typedef struct vector_t *Vector;


struct matrix_t {
    int rows;
    int cols;
    double* elements;
};

typedef struct matrix_t *Matrix;

double Uniform();
double normal_distribution_random_value(double mu, double sigma);
Vector create_vector(int size);
Matrix create_matrix(int rows, int cols);
Matrix create_matrix_from_file(const char *file_path, int rows, int cols);
Matrix create_matrix_at_random(int rows, int cols);
Vector create_vector_from_file(const char *file_path, int size);
Matrix create_matrix_with_ptr(int rows, int cols, double *elements);
Vector add_vector(const Vector a, const Vector b);
Matrix add_matrix(const Matrix a, const Matrix b);
Matrix add_vector_matrix(const Vector v, const Matrix M);
Vector dot_vector_matrix(const Vector v, const Matrix M);
Matrix dot_matrix(const Matrix M, const Matrix N);
Matrix quick_dot_matrix(const Matrix A_M, const Matrix B_M, const Matrix C_M, double alpha, double beta);
Matrix product_vector_matrix(const Vector v, const Matrix M);
Matrix product_matrix(const Matrix M, const Matrix N);
Vector product_vector(const Vector v, const Vector u);
Vector product_scalar_vector(const double a, const Vector v);
Matrix product_scalar_matrix(const double a, const Matrix M);

Matrix transpose(const Matrix M);
Vector copy_vector(const Vector v);
Matrix copy_matrix(const Matrix M);
void save_matrix(char* file_name, const Matrix M);
void free_vector(Vector v);
void free_matrix(Matrix M);
void print_vector(Vector v);
void print_matrix(Matrix M);
bool is_same_matrix(const Matrix M, const Matrix N);

double max_value_in_vector(const Vector v);
double max_value_in_row(const Matrix M, int row);
double sum_values_in_vector(const Vector v);
int vector_argmax(const Vector v);
Vector matrix_argmax(const Matrix M);
double sum_value_in_matrix(const Matrix M);
Vector sum_value_in_cols(const Matrix M);

int *choice(int size, int num);
Matrix matrix_reshape(const Matrix M, int rows, int cols);
Vector matrix_row_max(const Matrix M);
int *matrix_argmax_row(const Matrix M);
Matrix create_label_batch(Matrix labels, const int *batch_index, int size);
void save_vector(char *file_name, Vector v);
Vector create_label_batch_vector(Vector labels, const int *batch_index, int size);
#endif