#include "gradient_check.h"

#include "Game.h"
#include "game_ai.h"
#include "AI.h"
#include "tensor.h"
#include "data_generator.h"
#include "value_net.h"
#include "trainer.h"
#include "util.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
static const int TRAIN_SIZE = 5000;
static const int TEST_SIZE = 1000;
static const int EPOCHS = 20;
static const int MINI_BATCH_SIZE = 100;
static const double LEARNING_RATE = 0.001;
static const double BETA1 = 0.9;
static const double BETA2 = 0.999;

void gradient_check()
{
  
    // AI mcs = new_ai("mcs",mcs_ai);
    // AI mcts = new_ai("mcts",mcts_ai);
    // evaluate_strength(mcs,mcts);
    // genenrate_data("../data/train_data.dat", "../data/train_labels.dat", 100);
    // genenrate_data("../data/test_data.dat", "../data/test_labels.dat", 10);
    char *train_data_file = "data/train_data.dat";
    char *train_label_file = "data/train_labels.dat";
    char *test_data_file = "data/test_data.dat";
    char *test_label_file = "data/test_labels.dat";

    Tensor x_train = load_x_train_data(train_data_file);
    Vector x_label = load_x_label_data(train_label_file);
    Tensor t_test = load_t_test_data(test_data_file);
    Vector t_label = load_t_label_data(test_label_file);

    int train_size = count_lines(train_data_file) / (5 * 40);
    int test_size = count_lines(test_data_file) / (5 * 40);

    ValueNet net = new_value_net(40, 5, 5, 3, 3, 1, 1, 0.2,0.5,0.2);

   

    int batch_index[1] = {0};
    Tensor x_batch = create_data_batch_tensor(x_train, batch_index, 1);
    Vector t_batch = create_label_batch_vector(x_label, batch_index, 1);

    Params grads_b = net->gradient(net, x_batch, t_batch);
    Params grads_n = net->numerical_gradient(net, x_batch, t_batch);
    
    save_params("data/backprop",grads_b);
    save_params("data/numerical",grads_n);
    Tensor W_b,W_n;
    Vector b_b,b_n;
    double W_error_conv = 0;
    double b_error_conv = 0;
    for(int m = 0; m < CONV_DEPTH; m++){
        W_b = grads_b->W[m];
        W_n = grads_n->W[m];

        for(int i = 0; i < W_b->num; i++){
            for(int j = 0; j < W_b->chs; j++){
                for(int k = 0; k < W_b->rows; k++){
                    for(int l = 0; l < W_b->cols; l++){
                        double b = read_tensor(W_b, i, j, k, l);
                        double n = read_tensor(W_n, i, j, k, l);

                        W_error_conv += fabs(b - n);
                    }
                }
            }
        }

        b_b = grads_b->b[m];
        b_n = grads_n->b[m];

        for(int i = 0; i < b_b->size; i++){
            b_error_conv += fabs(b_b->elements[i] - b_n->elements[i]);
        }
        printf("Conv W%d: %lf\n",m, W_error_conv/(W_b->num*W_b->chs*W_b->rows*W_b->cols));
        printf("Conv b%d: %lf\n",m, b_error_conv/(b_b->size));

    }

    double W_error_affine1 = 0;
    double W_error_affine2 = 0;
    double b_error_affine1 = 0;
    double b_error_affine2 = 0;

    Matrix W1_b = grads_b->W1;
    Matrix W1_n = grads_n->W1;
    Matrix W2_b = grads_b->W2;
    Matrix W2_n = grads_n->W2;

    Vector b1_b = grads_b->b1;
    Vector b1_n = grads_n->b1;
    Vector b2_b = grads_b->b2;
    Vector b2_n = grads_n->b2;
    
    for(int i = 0; i < W1_b->rows; i++){
        for(int j = 0; j < W1_b->cols; j++){
            W_error_affine1 += fabs(W1_b->elements[i * W1_b->cols * j] - W1_n->elements[i * W1_n->cols +j]);
        }
    }

    for(int i = 0; i < b1_b->size; i++){
        b_error_affine1 += fabs(b1_b->elements[i] - b1_n->elements[i]);
    }

    printf("Affine W1: %lf\n",W_error_affine1/(W1_b->rows*W1_b->cols));
    printf("Affine b1: %lf\n",b_error_affine1/(b1_b->size));

    for(int i = 0; i< W2_b->rows; i++){
        for(int j = 0; j < W2_b->cols; j++){
            W_error_affine2 += fabs(W2_b->elements[i * W2_b->cols * j] - W2_n->elements[i * W2_n->cols + j]);
        }
    }

    for (int i = 0; i < b2_b->size; i++)
    {
        b_error_affine2 += fabs(b2_b->elements[i] - b2_n->elements[i]);
    }

    printf("Affine W2: %lf\n", W_error_affine2 / (W2_b->rows * W2_b->cols));
    printf("Affine b2: %lf\n", b_error_affine2 / (b2_b->size));


    // print_data(x_train);
    // Game game = new_game(FIRST);
    // game->cpu_vs_cpu(game, random_ai, mcts_ai);
}