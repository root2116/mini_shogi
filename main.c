#include "Game.h"
#include "game_ai.h"
#include <stdlib.h>
int main(){
    srand(42);
    Game game = new_game(FIRST);
    evaluate_strength(game,random_ai,mcs_ai);
    return 0;
}