#ifndef MASK_H
#define MASK_H
#include <stdbool.h>


typedef struct mask_t *Mask;
struct mask_t
{
    int rows;
    int cols;
    bool *elements;
};

typedef struct mask_tensor_t *MaskTensor;
struct mask_tensor_t
{
    int num;
    int chs;
    int rows;
    int cols;
    bool ****elements;
};

Mask create_mask(int rows, int cols);
MaskTensor create_mask_tensor(int num, int chs, int rows, int cols);
void free_mask(Mask m);
void free_mask_tensor(MaskTensor m);

#endif