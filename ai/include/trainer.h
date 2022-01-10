
#ifndef TRAINER_H
#define TRAINER_H
#include <stdint.h>
#include <stdbool.h>
#include "affine.h"
#include "relu.h"
#include "matrix.h"
#include "tensor.h"
#include "convolution.h"
#include "value_net.h"
#include "relu_tensor.h"
#include "affine_tensor.h"
#include "list_double.h"
#include "optimizer.h"






struct trainer_t
{
    ValueNet net;
    Optimizer optimizer;
    Tensor train_data;
    Vector train_labels;
    Tensor test_data;
    Vector test_labels;
    int epochs;
    int optimizer_type;
    int mini_batch_size;
    int train_size;
    int test_size;
    double learning_rate;
    bool verbose;
    int iter_per_epoch;
    int max_iter;
    ListW train_loss_list;
    ListW train_acc_list;
    ListW test_acc_list;
    int current_iter;
    int current_epoch;
    void (*train)();

};

typedef struct trainer_t *Trainer;

Trainer new_trainer(ValueNet net,
                    Tensor train_data,
                    Vector train_labels,
                    Tensor test_data,
                    Vector test_labels,
                    int epochs,
                    int optimizer_type,
                    int mini_batch_size,
                    int train_size,
                    int test_size,
                    double learning_rate,
                    bool verbose);

#endif