#include "Game.h"
#include "game_ai.h"
#include <stdlib.h>
int main(){
    srand(42);
    
    evaluate_strength(random_ai,mcs_ai);
    return 0;
}