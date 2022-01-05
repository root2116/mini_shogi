#include "Game.h"
#include "game_ai.h"
#include "AI.h"
#include "tensor.h"
#include "data_generator.h"
#include "value_net.h"
#include <unistd.h>
#include <stdlib.h>


int main(){
    srand(42);
    // AI mcs = new_ai("mcs",mcs_ai);
    // AI mcts = new_ai("mcts",mcts_ai);
    // evaluate_strength(mcs,mcts);
    // genenrate_data("../data/data.dat","../data/labels.dat",10);
    Tensor x_train = load_x_train_data("data/data.dat");
    Vector x_label = load_x_label_data("data/labels.dat");

    int batch_index[3] = {0, 1, 2};
    Tensor x_batch = create_data_batch_tensor(x_train, batch_index, 3);
    Vector t_batch = create_label_batch_vector(x_label, batch_index, 3);

    ValueNet net = new_value_net(40, 5, 5, 80, 3, 1, 1,0.01);
    net->gradient(net, x_batch, t_batch);

    // print_data(x_train);
    // Game game = new_game(FIRST);
    // game->cpu_vs_cpu(game, random_ai, mcts_ai);
    return 0;
}