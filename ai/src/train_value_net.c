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
static const int TRAIN_SIZE = 5000;
static const int TEST_SIZE = 1000;
static const int EPOCHS = 20;
static const int MINI_BATCH_SIZE = 100;
static const double LEARNING_RATE = 0.001;
static const double BETA1 = 0.9;
static const double BETA2 = 0.999;




void train_value_net()
{
    srand((unsigned)time(NULL));
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
    int test_size = count_lines(test_data_file)/ (5 * 40);

    ValueNet net = new_value_net(40, 5, 5, 80, 3, 1, 1,0.2,0.5,0.2, 96);

    Trainer trainer = new_trainer(net,x_train, x_label, t_test, t_label,EPOCHS,Adam , MINI_BATCH_SIZE, train_size, test_size, LEARNING_RATE, true);

    trainer->train(trainer);

    // print_data(x_train);
    // Game game = new_game(FIRST);
    // game->cpu_vs_cpu(game, random_ai, mcts_ai);
  
}