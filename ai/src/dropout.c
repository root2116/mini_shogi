#include "dropout.h"
#include "matrix.h"

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


static Matrix forward(Dropout this, const Matrix X, bool is_backprop)
{
    if (is_backprop)
    {
        
        if(this->mask != NULL){
            free_mask(this->mask);
        }

        Matrix RND = create_matrix_at_random(X->rows, X->cols);
        
        Matrix M = create_matrix(X->rows, X->cols);
        this->mask = create_mask(X->rows, X->cols);

        for (int i = 0; i < M->rows; ++i)
        {
            for (int j = 0; j < M->cols; ++j)
            {
                if (RND->elements[i * RND->cols + j] > this->dropout_ratio)
                {
                    this->mask->elements[i * this->mask->cols + j] = true;
                    M->elements[i * M->cols + j ] = X->elements[i * X->cols + j];
                }
                else
                {
                    this->mask->elements[i * this->mask->cols + j] = false;
                    M->elements[i * M->cols + j ] = 0;
                }
            }
        }

        free_matrix(RND);

        return M;
    }
    else
    {
        Matrix M = product_scalar_matrix(1.0 - this->dropout_ratio, X);
        return M;
    }
}

static Matrix backward(const Dropout this, const Matrix X)
{
    Matrix M = create_matrix(X->rows, X->cols);
    for (int i = 0; i < M->rows; ++i)
    {
        for (int j = 0; j < M->cols; ++j)
        {
            if (this->mask->elements[i * this->mask->cols + j])
            {
                M->elements[i * M->cols + j] = X->elements[i * X->cols + j];
            }
            else
            {
                M->elements[i * M->cols + j] = 0;
            }
        }
    }

    return M;
}

Dropout new_dropout(double dropout_ratio)
{
    Dropout d = malloc(sizeof(*d));
    d->dropout_ratio = dropout_ratio;
    d->mask = NULL;

    d->forward = forward;
    d->backward = backward;
    return d;
}
