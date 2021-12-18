#include "Game.h"
#include "game_ai.h"
int main(){
    Game game = new_game(FIRST);
    game->cpu_vs_cpu(game,random_ai,mcs_ai);
    return 0;
}