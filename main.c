#include "Game.h"
#include "game_ai.h"
#include "AI.h"
#include "tensor.h"
#include "data_generator.h"
#include "value_net.h"
#include "trainer.h"
#include "util.h"
#include "train_value_net.h"
#include "gradient_check.h"

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

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
static const int TRAIN_SIZE = 5000;
static const int TEST_SIZE = 1000;
static const int EPOCHS = 20;
static const int MINI_BATCH_SIZE = 32;
static const double LEARNING_RATE = 0.001;
static const double BETA1 = 0.9;
static const double BETA2 = 0.999;


int main(){
    srand((unsigned) time(NULL));
    // srand(42);
    init();
    // train_value_net();
    // AI mcs = new_ai("mcs",mcs_ai);
    // AI mcts = new_ai("mcts",mcts_ai);
    // evaluate_strength(mcs,mcts);
  
    // gradient_check();
    
    // char train_data_file[32] = "data/train_data_all.dat";
    // char train_label_file[32] = "../data/train_labels_all.dat";
    // char test_data_file[32] = "../data/test_data_all.dat";
    // char test_label_file[32] = "../data/test_labels_all.dat";

    char train_data_file[128];
    char train_label_file[128]; 
    char test_data_file[128];
    char test_label_file[128];
    sprintf(train_data_file, "%s/data/train_data1.dat", dirname(__FILE__));
    sprintf(train_label_file, "%s/data/train_labels1.dat", dirname(__FILE__));
    sprintf(test_data_file, "%s/data/test_data_1", dirname(__FILE__));
    sprintf(test_label_file, "%s/data/test_labels1.dat", dirname(__FILE__));

    genenrate_data(train_data_file,train_label_file,1000);
    genenrate_data(test_data_file,test_label_file,100);

    // Tensor x_train = load_x_train_data(train_data_file);
    // Vector x_label = load_x_label_data(train_label_file);
    // Tensor t_test = load_t_test_data(test_data_file);
    // Vector t_label = load_t_label_data(test_label_file);

    
    // int train_size = count_lines(train_data_file) / (5 * 40);
    // int test_size = count_lines(test_data_file)/ (5 * 40);

    // ValueNet net = new_value_net(40, 5, 5, 32, 3, 1, 1, 0, 0, 0, 32);

    
    // Trainer trainer = new_trainer(net,x_train, x_label, t_test, t_label,EPOCHS,Adam , MINI_BATCH_SIZE, train_size, test_size, LEARNING_RATE, true);

    // trainer->train(trainer);

    // save_params("params",net->params);
    // save_list_w("train_loss.dat",trainer->train_loss_list);
    // save_list_w("train_acc.dat",trainer->train_acc_list);
    // save_list_w("test_acc.dat",trainer->test_acc_list);

  



    // print_data(x_train);
    // Game game = new_game(FIRST);
    // game->cpu_vs_cpu(game, random_ai, mcts_ai);
    return 0;
}