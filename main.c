#include "Game.h"
#include "game_ai.h"
#include "AI.h"
#include <time.h>

#include <stdlib.h>
int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    
    Game game = new_game(atoi(argv[1]));
    game->user_vs_cpu(game,mcts_ai);

    return 0;
}