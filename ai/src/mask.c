#include "mask.h"
#include "matrix.h"
#include <stdlib.h>
#include <stdbool.h>

void free_mask(Mask m)
{

    free(m->elements);
    free(m);
}

void free_mask_tensor(MaskTensor m)
{
    for (int i = 0; i < m->num; ++i)
    {
        for (int j = 0; j < m->chs; ++j)
        {
            for (int k = 0; k < m->rows; ++k)
            {
                free(m->elements[i][j][k]);
            }
            free(m->elements[i][j]);
        }
        free(m->elements[i]);
    }
    free(m->elements);
    free(m);
}

Mask create_mask(int rows, int cols)
{
    Mask m = malloc(sizeof(*m));
    m->rows = rows;
    m->cols = cols;
    m->elements = malloc(rows*cols*sizeof(bool));
    
    return m;
}

MaskTensor create_mask_tensor(int num, int chs, int rows, int cols)
{
    MaskTensor m = malloc(sizeof(*m));
    m->num = num;
    m->chs = chs;
    m->rows = rows;
    m->cols = cols;

    m->elements = calloc(num, sizeof(bool ***));
    for (int i = 0; i < num; ++i)
    {
        m->elements[i] = calloc(chs, sizeof(bool **));
        for (int j = 0; j < chs; ++j)
        {
            m->elements[i][j] = calloc(rows, sizeof(bool *));
            for (int k = 0; k < rows; ++k)
            {
                m->elements[i][j][k] = calloc(cols, sizeof(bool));
            }
        }
    }

    return m;
}