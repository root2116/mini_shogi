#include "Game.h"
#include "game_ai.h"
#include "AI.h"


#include <stdlib.h>
int main(){
    // srand(42);
    // AI mcs = new_ai("mcs",mcs_ai);
    // AI mcts = new_ai("mcts",mcts_ai);
    // evaluate_strength(mcs,mcts);
    Game game = new_game(FIRST);
    game->user_vs_user(game);
    return 0;
}