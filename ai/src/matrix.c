#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <string.h>


double Uniform()
{
    return ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
};

double normal_distribution_random_value(double mu, double sigma)
{

    double z = sqrt(-2.0 * log(Uniform())) * sin(2.0 * M_PI * Uniform());

    return mu + sigma * z;
}

Vector create_vector(int size){
    Vector new_vec = malloc(sizeof(*new_vec));
    new_vec->size = size;
    new_vec->elements = malloc(size*sizeof(double));

    for(int i = 0; i < size; i++){
        new_vec->elements[i] = 0;
    }

    return new_vec;

}

Vector create_vector_from_file(const char* file_path, int size){
    Vector new_vec = create_vector(size);

    FILE *fp = fopen(file_path, "r");
    char data[1024];
    int i = 0;
    while(fgets(data,1024,fp) != NULL){
        new_vec->elements[i] = atof(data);
        i++;
        
    }

    return new_vec;
}

Vector create_vector_from_binary_file(const char* file_path, int size){
    FILE *fp = fopen(file_path, "rb");

    double *elements = malloc(size*sizeof(double));
    fread(elements, sizeof(double), size, fp);

    fclose(fp);

    Vector v = malloc(sizeof(*v));

    v->size = size;
    v->elements = elements;

    return v;
}

Matrix create_matrix(int rows, int cols){
    Matrix new_matrix = malloc(sizeof(*new_matrix));

    new_matrix->rows = rows;
    new_matrix->cols = cols;

    new_matrix->elements = malloc(cols*rows*sizeof(double*));

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            new_matrix->elements[i * cols + j] = 0;
        }
    }
    
    
    return new_matrix;
}

Matrix create_matrix_with_ptr(int rows, int cols, double *elements){
    Matrix new = malloc(sizeof(*new));
    
    new->rows = rows;
    new->cols = cols;

    new->elements = elements;

    return new;
}

Matrix create_matrix_from_file(const char *file_path, int rows, int cols){
    FILE *fp = fopen(file_path,"r");

    char data[4096];
    Matrix new_matrix = create_matrix(rows, cols);
    int i = 0;
    int j = 0;

    while(fgets(data,12048,fp) != NULL){
        j = 0;
        char *p = data;
        char *head = data;
        while(*p != '\0'){
            head = p;
            while(*p != ' ' && *p != '\n'){
                p++;   
            }
            *p = '\0';
            new_matrix->elements[i * new_matrix->cols + j] = atof(head);
            p++;
            j++;
        }
        i++;
    }

    return new_matrix;
}

Matrix create_matrix_from_binary_file(const char *file_name, int rows, int cols){
    FILE *fp = fopen(file_name, "rb");

    double *elements = malloc(rows*cols*sizeof(double));
    fread(elements, sizeof(double), rows*cols, fp);

    fclose(fp);

    Matrix M = malloc(sizeof(*M));

    M->rows = rows;
    M->cols = cols;
    M->elements = elements;

    return M;
}

Matrix create_matrix_at_random(int rows, int cols){

    Matrix M = create_matrix(rows,cols);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            M->elements[i * M->cols + j] = normal_distribution_random_value(0,1);
        }
    }
    
    return M;
}

Vector add_vector(const Vector a, const Vector b){

    if(a->size != b->size){
        fprintf(stderr,"Not same size.(add_vector)\n");
        return NULL;
    }

    int size = a->size;
    Vector c = create_vector(size);
    
    for(int i = 0; i < size; i++){
        c->elements[i] = a->elements[i] + b->elements[i];
    }

    return c;
}

Matrix add_matrix(const Matrix a, const Matrix b){
    if(a->rows != b->rows || a->cols != b->cols){
        fprintf(stderr, "Not same size.(add_matrix)\n");
        return NULL;
    }

    int rows = a->rows;
    int cols = a->cols;
    Matrix c = create_matrix(rows, cols);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            c->elements[i * c->cols + j] = a->elements[i * a->cols + j] + b->elements[i * b->cols +j];
        }
    }

    return c;
}

//テスト済み
Matrix add_vector_matrix(const Vector v, const Matrix M){
    if(v->size != M->cols){
        fprintf(stderr, "Not same size.(add_vector_matrix)\n");
        return NULL;
    }

    Matrix r = create_matrix(M->rows, M->cols);

    for(int i = 0; i < M->rows; i++){
        for(int j = 0; j < M->cols; j++){
            r->elements[i * r->cols + j] = v->elements[j] + M->elements[i * M->cols + j];
        }
    }

    return r;
}
Vector dot_vector_matrix(const Vector v, const Matrix M){

    if(M->rows != v->size){
        fprintf(stderr, "Invalid size.(dot_vector_matrix)\n");
        return NULL;
    }
    Vector result = create_vector(M->cols);

    for (int i = 0; i < result->size; i++){
        for(int j = 0; j < M->rows ; j++){
            result->elements[i] += v->elements[j] * M->elements[j * M->cols + i];
        }
    }

    return result;
}

Matrix dot_matrix(const Matrix M, const Matrix N){
    if(M->cols != N->rows){
        fprintf(stderr, "Invalid size.(dot_matrix)\n");
        return NULL;
    }

    Matrix result = create_matrix(M->rows,N->cols);
    // #pragma omp parallel for private(j, k)
    for(int i = 0; i < M->rows; i++){
        for(int k = 0; k < M->cols; k++){
            for(int j = 0; j < N->cols; j++){
                result->elements[i* result->cols + j] += M->elements[i * M->cols + k] * N->elements[k * N->cols + j];
            }
            
        }
    }

    return result;
}



//テスト済み
Vector product_scalar_vector(const double a, const Vector v){
    Vector result = create_vector(v->size);
    for(int i = 0; i < v->size; i++){
        result->elements[i] = a * v->elements[i];
    }

    return result;
}

//テスト済み
Matrix product_scalar_matrix(const double a, const Matrix M){
    Matrix result = create_matrix(M->rows, M->cols);

    for(int i = 0; i < M->rows; i++){
        for(int j = 0; j < M->cols; j++){
            result->elements[i * result->cols +j] = a * M->elements[i * M->cols + j];
        }
    }

    return result;
}
Matrix transpose(const Matrix M){
    Matrix transposed = create_matrix(M->cols,M->rows);

    for(int i = 0; i < M->cols; i++){
        for(int j = 0; j < M->rows; j++){
            transposed->elements[i * transposed->cols + j] = M->elements[j * M->cols + i];
        }
    }

    return transposed;
}


Vector copy_vector(const Vector v){
    Vector copy = create_vector(v->size);

    for(int i = 0; i < v->size; i++){
        copy->elements[i] = v->elements[i];
    }

    return copy;
}

Matrix copy_matrix(const Matrix M){
    Matrix copy = create_matrix(M->rows, M->cols);

    for(int i = 0; i < M->rows; i++){
        for(int j = 0; j < M->cols; j++){
            copy->elements[i * copy->cols + j] = M->elements[i * M->cols + j];
        }
    }

    return copy;
}



void save_matrix(char* file_name, const Matrix M){
    FILE *fp = fopen(file_name, "w");

    for(int i = 0; i < M->rows; i++){
        for(int j = 0; j < M->cols; j++){
            double value = M->elements[i * M->cols + j];
            fprintf(fp, "%lf ",value);
        }
        fprintf(fp,"\n");
    }

    fclose(fp);
    
}

void save_matrix_as_binary(char *file_name, const Matrix M){

    FILE *fp = fopen(file_name, "wb");

    fwrite(M->elements,sizeof(double), M->rows*M->cols, fp);

    fclose(fp);
}

void free_vector(Vector v){
    free(v->elements);
    free(v);
}

void free_matrix(Matrix M){


    free(M->elements);
    free(M);

}

void print_vector(Vector v){

    printf("[");
    for(int i = 0; i < v->size; i++){
        printf("%lf,",v->elements[i]);
    }
    printf("\b]\n");


}

void print_matrix(Matrix M){

    printf("[");
    for(int i = 0; i < M->rows; i++){

        if(i == 0){
            printf("[");
        }else{
            printf(" [");
        }

        for(int j = 0; j < M->cols; j++){
            printf("%lf,",M->elements[i * M->cols + j]);
        }
        if(i == M->rows - 1){
            printf("\b]");
        }else{
            printf("\b],\n");
        }
        
    }
    printf("]\n");
}


bool is_same_matrix(const Matrix M, const Matrix N){
    if(M->rows != N->rows || M->cols != N->cols){
        return false;
    }

    for(int i = 0; i < M->rows;i++){
        for(int j = 0; j < M->cols; j++){
            if(M->elements[i * M->cols + j] != N->elements[i * N->cols + j]){
                return false;
            }
        }
    }

    return true;
}


double max_value_in_vector(const Vector v){
    double max = v->elements[0];
    for(int i = 0; i < v->size; i++){
        if(max < v->elements[i]){
            max = v->elements[i];
        }
    }

    return max;
}

double max_value_in_row(const Matrix M, int row){
    double max = M->elements[row * M->cols + 0];

    for(int i = 0; i < M->cols; i++){
        if(max < M->elements[row * M->cols + i]){
            max = M->elements[row * M->cols + i];
        }
    }

    return max;
}

double sum_values_in_vector(const Vector v){
    double sum = 0;
    for(int i = 0; i < v->size; i++){
        sum += v->elements[i];
    }

    return sum;
}

int vector_argmax(const Vector v){
    double max = v->elements[0];
    int index = 0;
    for(int i = 0; i < v->size; i++){
        if(max < v->elements[i]){
            max = v->elements[i];
            index = i;
        }
    }

    return index;
}

Vector matrix_argmax(const Matrix M){

    Vector result = create_vector(M->rows);
    for(int i = 0; i < M->rows; i++){
        double max = M->elements[i * M->cols + 0];
        int max_index = 0;
        for(int j = 0; j < M->cols; j++){
            if(max < M->elements[i * M->cols + j]){
                max = M->elements[i * M->cols + j];
                max_index = j;
            }
        }

        result->elements[i] = max_index;
    }

    return result;
}

double sum_value_in_matrix(const Matrix M){
    int sum = 0;

    for(int i = 0; i < M->rows; i++){
        for(int j = 0; j < M->cols; j++){
            sum += M->elements[i * M->cols + j];
        }
    }

    return sum;
}

Vector sum_value_in_cols(const Matrix M){
    Vector sum = create_vector(M->cols);

    

    for(int i = 0; i < M->cols; i++){
        double sum_value = 0;
        for(int j = 0; j < M->rows; j++){
            sum_value += M->elements[j * M->cols + i];
            
        }
        sum->elements[i] = sum_value;
    }

    return sum;


}
int *choice(int size, int num)
{
    
    if (size < num)
    {
        fprintf(stderr, "Invalid size.\n");
        return NULL;
    }

    int vals[size];
    for (int i = 0; i < size; ++i)
    {
        vals[i] = i;
    }

    // shuffle
    for (int i = 0; i < size; ++i)
    {
        int j = rand() % size;
        int t = vals[i];
        vals[i] = vals[j];
        vals[j] = t;
    }

    int *ret = malloc(sizeof(int) * num);
    memcpy(ret, vals, sizeof(int) * num);

    return ret;
}

Matrix matrix_reshape(const Matrix M, int rows, int cols)
{
    int r = rows;
    int c = cols;
    if (rows < 0)
    {
        r = (M->rows * M->cols) / cols;
    }
    else if (cols < 0)
    {
        c = (M->rows * M->cols) / rows;
    }

    int r_pos = 0, c_pos = 0;
    Matrix R = create_matrix(r, c);

    for (int i = 0; i < M->rows; i++){
        for (int j = 0; j < M->cols; j++){
            R->elements[r_pos * R->cols + c_pos++] = M->elements[i * M->cols + j];
            if (c_pos == c)
            {
                c_pos = 0;
                ++r_pos;
            }
        }
    }

    return R;
}

Vector matrix_row_max(const Matrix M) {
    Vector v = create_vector(M->rows);

    for (int i = 0; i < M->rows; ++i) {
        double max_val = -DBL_MAX;
        for (int j = 0; j < M->cols; ++j) {
            max_val = fmax(max_val, M->elements[i* M->cols + j]);
        }
        v->elements[i] = max_val;
    }

    return v;
}


int* matrix_argmax_row(const Matrix M) {
    int* arg_max = malloc(sizeof(int) * M->rows);

    for (int i = 0; i < M->rows; ++i) {
        int idx = 0;
        double max = -DBL_MAX;
        for (int j = 0; j < M->cols; ++j) {
            if (max < M->elements[i * M->cols + j]) {
                idx = j;
                max = M->elements[i * M->cols + j];
            }
        }

        arg_max[i] = idx;
    }

    return arg_max;
}

Matrix create_label_batch(Matrix labels, const int *batch_index, int size)
{
    Matrix v = create_matrix(size,10);

    for (int i = 0; i < size; i++)
    {
        for(int j = 0; j < 10; j++){

            v->elements[i * v->cols + j] = labels->elements[batch_index[i] * labels->cols + j];
        }
        
    }

    return v;
};

Vector create_label_batch_vector(Vector labels, const int *batch_index, int size){
    Vector v = create_vector(size);

    for(int i = 0; i < size; i++){
        v->elements[i] = labels->elements[batch_index[i]];
    }

    return v;
}


void save_vector(char* file_name, Vector v){
    FILE *fp = fopen(file_name,"w");

    for(int i = 0; i < v->size; i++){
        fprintf(fp,"%lf\n",v->elements[i]);
    }

    fclose(fp);

}

void save_vector_as_binary(char* file_name, Vector v){
    FILE *fp = fopen(file_name, "wb");

    fwrite(v->elements, sizeof(double), v->size, fp);

    fclose(fp);

    
}


void check_validity(Vector t){
    for(int i = 0; i < t->size; i++){
        if(t->elements[i] != 0 && t->elements[i] != 1){
            printf("Invalid value found.\n");
            return;
        }
    }


}
