
#include "Gold.h"
#include "Piece.h"
#include "Game.h"
#include "Board.h"
#include "utility.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


Piece new_gold(int side){
    Piece instance = new_piece();


   

    strcpy(instance->name, "KI");
    strcpy(instance->eng_name, "GOLD");
    strcpy(instance->idle_eng_name, "GOLD");

    instance->kind = GOLD;
    instance->idle_kind = GOLD;
    
    instance->promoted = false;

    if (side == FIRST)
    {
        instance->cur_loc.x = 1;
        instance->cur_loc.y = 4;
    }
    else if (side == SECOND)
    {
        instance->cur_loc.x = 3;
        instance->cur_loc.y = 0;
    }

    int gold_array[GOLD_MOVE_NUM][2] = {
        {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{0,1}
        };

    

    if (side == FIRST){

        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->idle_ability.directions);
    }else if(side == SECOND){
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->idle_ability.directions);

        inverse_vectors(instance->ability.directions, GOLD_MOVE_NUM);
        inverse_vectors(instance->idle_ability.directions, GOLD_MOVE_NUM);
    }
    
    instance->ability.length = GOLD_MOVE_NUM;
    instance->idle_ability.length = GOLD_MOVE_NUM;

    instance->side = side;

    return instance;
}




