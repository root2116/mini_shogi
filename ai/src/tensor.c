#include "tensor.h"
#include "matrix.h"
#include "function.h"
#include <stdlib.h>


#define NUM_OF_ROWS 28
#define NUM_OF_COLS 28
#define NUM_OF_PIXELS 784
#define NUM_OF_TRAIN_IMAGES 60000
#define NUM_OF_TEST_IMAGES 10000

Tensor create_tensor(int num, int chs, int rows, int cols)
{
    Tensor new = malloc(sizeof(*new));

    new->num = num;
    new->chs = chs;
    new->rows = rows;
    new->cols = cols;
   
    

    new->elements = malloc(rows*cols*chs*num*sizeof(double));

    for(int num_i = 0; num_i < new->num; num_i++){
        for(int ch_i = 0; ch_i < new->chs; ch_i++){
            for(int row_i = 0; row_i < new->rows; row_i++){
                for(int col_i = 0; col_i < new->cols; col_i++){
                    write_tensor(0,new,num_i, ch_i, row_i, col_i);
                }
            }
        }
    }
    


    return new;

}

Tensor create_tensor_at_random(int num, int chs, int rows, int cols){

    Tensor R = create_tensor(num,chs,rows,cols);
    for(int i = 0; i < num; i++){
        for(int j = 0; j < chs; j++){
            for(int k = 0; k < rows; k++){
                for(int l = 0; l < cols; l++){
                    double value = normal_distribution_random_value(0,1);

                    write_tensor(value,R,i,j,k,l);
                }
            }
        }
    }

    return R;
}

void free_tensor(Tensor tensor){
    free(tensor->elements);
    free(tensor);
}


void write_tensor(double value, Tensor tensor, int n, int ch, int row, int col){

    tensor->elements[ tensor->chs * tensor->rows * tensor->cols * n + tensor->rows * tensor->cols * ch + tensor->cols * row + col] = value;

}

double read_tensor(Tensor tensor, int n, int ch, int row, int col){
    return tensor->elements[tensor->chs * tensor->rows * tensor->cols * n + tensor->rows * tensor->cols * ch + tensor->cols * row + col];
}

double *address_tensor(Tensor tensor, int n, int ch, int row, int col){
    return &(tensor->elements[tensor->chs * tensor->rows * tensor->cols * n + tensor->rows * tensor->cols * ch + tensor->cols * row + col]);

}

//必要になったときに理解する
Matrix im2col(Tensor input_data, int filter_h, int filter_w, int stride, int pad)
{
    int N = input_data->num;
    int C = input_data->chs;
    int H = input_data->rows;
    int W = input_data->cols;

    int out_h = (H + 2 * pad - filter_h)/stride + 1;
    int out_w = (W + 2 * pad - filter_w)/stride + 1;

    Tensor img = pad_tensor(input_data, pad);

    Matrix R = create_matrix(N * out_h * out_w, filter_h * filter_w * C);

    int rpos = 0, cpos = 0; 
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < out_h * stride; k += stride) {
            for (int l = 0; l < out_w * stride; l += stride) {
                for (int j = 0; j < C; ++j) {
                    for (int m = k; m < k + filter_w; ++m) {
                        for (int n = l; n < l + filter_h; ++n) {
                            R->elements[rpos * R->cols + cpos] = read_tensor(img, i, j, m, n);

                            ++ cpos;
                            if (cpos == R->cols) {
                                cpos = 0;
                                ++rpos;
                            }
                        }
                    }
                }
            }
        }
    }

    return R;
}

Tensor pad_tensor(Tensor T, int pad)
{
    Tensor R = create_tensor(T->num, T->chs, T->rows + 2*pad, T->cols + 2*pad);

    for (int i = 0; i < R->num; i++){
        for (int j = 0; j < R->chs; j++){
            for (int k = 0; k < R->rows; k++){
                for (int l = 0; l < R->cols; l++){
                    if ((k < pad) || ((T->rows + pad) <= k) || (l < pad) || ((T->cols + pad) <= l)){
                        
                        write_tensor(0,R,i,j,k,l);
                    }
                    else{

                        double value = read_tensor(T,i,j,k-pad,l-pad);
                        write_tensor(value, R, i,j,k,l);
                    }
                }
            }
        }
    }

    return R;
}

Matrix reshape_to_matrix(const Tensor T, int rows, int cols) {
    int r = rows;
    int c = cols;
    if (rows < 0) {
        r = T->num * T->chs * T->rows * T->cols / cols;
    } else if (cols < 0) {
        c = T->num * T->chs * T->rows * T->cols / rows;
    }

    Matrix R = create_matrix(r, c);
    int r_pos = 0, c_pos = 0;
    for (int i = 0; i < T->num; i++) {
        for (int j = 0; j < T->chs; j++) {
            for (int k = 0; k < T->rows; k++) {
                for (int l = 0; l < T->cols; l++) {
                    R->elements[r_pos * R->cols + c_pos++] = 
                    read_tensor(T,i,j,k,l);
                    if (c_pos == c) {
                        c_pos = 0;
                        ++r_pos;
                    }
                }
            }
        }
    }

    return R;
}


Tensor reshape_to_tensor(const Matrix M, int num, int chs, int rows, int cols) {
    int sizes[] = {num, chs, rows, cols};
    if (cols < 0) {
        sizes[3] = M->rows * M->cols / (num * chs * rows);
    }

    
    Tensor R = create_tensor(sizes[0],sizes[1],sizes[2],sizes[3]);

    int p1 = 0, p2 = 0, p3 = 0, p4 = 0;
    for (int i = 0; i < M->rows; ++i) {
        for (int j = 0; j < M->cols; ++j){

            write_tensor(M->elements[i * M->cols + j] ,R, p1, p2, p3, p4);

            ++p4;
            if (p4 == sizes[3]) {
                p4 = 0;
                ++p3;
            }

            if (p3 == sizes[2]) {
                p3 = 0;
                ++p2;
            }

            if (p2 == sizes[1]) {
                p2 = 0;
                ++p1;
            } 
        }
    }

    return R;
}


Tensor tensor_transpose(const Tensor M, int n1, int n2, int n3, int n4) {

    int sizes[4] = {M->num, M->chs, M->rows, M->cols};
    Tensor R = create_tensor(sizes[n1], sizes[n2], sizes[n3], sizes[n4]);

    int idx[4] = {0};
    int n[4] = {n1, n2, n3, n4};
    for (int i = 0; i < M->num; i++) {
        for (int j = 0; j < M->chs; j++) {
            for (int k = 0; k < M->rows; k++) {
                for (int l = 0; l < M->cols; l++) {
                    const int org[] = {i, j, k, l};

                    for (int i = 0; i < 4; ++i) {
                        idx[i] = org[n[i]];
                    }

                    double value = read_tensor(M, i, j, k, l);
                    write_tensor(value, R, idx[0], idx[1], idx[2], idx[3]);
                }
            }
        }
    }

    return R;
}


Tensor col2im(const Matrix M, int num, int chs, int rows, int cols, int filter_h, int filter_w, int stride, int pad) {
    const int N = num;
    const int C = chs; 
    const int H = rows;
    const int W = cols;

    const int out_h = (H + 2 * pad - filter_h) / stride + 1; 
    const int out_w = (W + 2 * pad - filter_w) / stride + 1; 
    Tensor B = create_tensor(N, C, H + 2 * pad + stride - 1, W + 2 * pad + stride - 1);

    int n = 0, c = 0, h = 0, w = 0;

    for (int i = 0; i < M->rows; ++i) {
        for (int j = 0; j < M->cols; j += (filter_h * filter_w)) {
            double* buf = malloc(sizeof(double) * (filter_h * filter_w));
            for (int k = j; k < j + (filter_h * filter_w); ++k) {
                buf[k-j] = M->elements[i * M->cols + k];
            }

            int idx = 0;
            for (int k = h; k < h + filter_h; ++k) {
                for (int l = w; l < w + filter_w; ++l) {
                    double value = read_tensor(B,n,c,k,l);
                    write_tensor(value + buf[idx++],B, n, c, k, l);
                }
            }

            ++c;
            if (c == C) {
                c = 0;
                w += stride;
            }

            if (w >= (out_w * stride)) {
                w = 0;
                h += stride;
            }

            if (h >= (out_h * stride)) {
                h = 0;
                ++n;
            }

            free(buf);
        }
    }
    
    Tensor R = create_tensor(N, C, H, W);
    for (int i = 0; i < B->num; ++i) {
        for (int j = 0; j < B->chs; ++j) {
            for (int k = 0; k < B->rows; ++k) {
                for (int l = 0; l < B->cols; ++l) {
                    if ((k < pad) || ((R->rows + pad) <= k) || (l < pad) || ((R->cols + pad) <= l)) {
                        continue;
                    } 

                    double value = read_tensor(B,i,j,k,l);
                    write_tensor(value, R, i, j, k - pad, l - pad);
                }
            }
        }
    }

    free_tensor(B);
    
    return R;
}


Tensor vector_reshape_to_tensor(const Vector v, int num, int chs, int rows, int cols) {
    int sizes[] = {num, chs, rows, cols};
    if (cols < 0) {
        sizes[3] = v->size / (num * chs * rows);
    }

    Tensor R = create_tensor(sizes[0], sizes[1], sizes[2], sizes[3]);

    int p1 = 0, p2 = 0, p3 = 0, p4 = 0;
    for (int i = 0; i < v->size; ++i) {

        write_tensor(v->elements[i], R, p1, p2, p3, p4);
        ++p4;
        if (p4 == sizes[3]) {
            p4 = 0;
            ++p3;
        }

        if (p3 == sizes[2]) {
            p3 = 0;
            ++p2;
        }

        if (p2 == sizes[1]) {
            p2 = 0;
            ++p1;
        } 
    }

    return R;
}

Vector tensor_flatten(const Tensor T) {
    Vector v = create_vector(T->num * T->chs * T->rows * T->cols);

    int pos = 0;
    for (int i = 0; i < T->num; ++i) {
        for (int j = 0; j < T->chs; ++j) {
            for (int k = 0; k < T->rows; ++k) {
                for (int l = 0; l < T->cols; ++l) {
                    double value = read_tensor(T,i,j,k,l);
                    v->elements[pos++] = value;
                    
                }
            }
        }
    }

    return v;
}

Tensor scalar_tensor(Tensor T, double v) {

    Tensor R = create_tensor(T->num, T->chs, T->rows, T->cols);

    for (int i = 0; i < T->num; i++) {
        for (int j = 0; j < T->chs; j++) {
            for (int k = 0; k < T->rows; k++) {
                for (int l = 0; l < T->cols; l++) {
                    double value = read_tensor(T,i,j,k,l);
                    write_tensor(value*v,R, i,j,k,l);
                }
            }
       }
    }

    return R;
}



Tensor create_image_batch_tensor(double**** images, const int* batch_index, int size) {
    Tensor M = create_tensor(size, 1, NUM_OF_ROWS, NUM_OF_COLS);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < NUM_OF_ROWS; ++j) {
            for (int k = 0; k < NUM_OF_COLS; ++k) {

                write_tensor(images[batch_index[i]][0][j][k],M, i, 0, j, k);
            }
        }
    }

    return M;
}


Tensor create_data_batch_tensor(Tensor X, const int* batch_index, int size){
    Tensor T = create_tensor(size, 40, 5, 5);

    for(int i = 0; i < size; i++){
        for(int j = 0; j < 40; j++){
            for(int k = 0; k < 5; k++){
                for(int l = 0; l < 5; l++){
                    double value = read_tensor(T,batch_index[i],j,k,l);
                    write_tensor(value, T, i,j,k,l);
                }
            }
        }
    }

    return T;
}