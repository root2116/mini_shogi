
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


   

    strcpy(instance->m->name, "KI");
    strcpy(instance->m->eng_name, "GOLD");
    strcpy(instance->m->idle_eng_name, "GOLD");

    instance->m->kind = GOLD;
    instance->m->idle_kind = GOLD;
    
    instance->m->promoted = false;

    if (side == FIRST)
    {
        instance->m->cur_loc.x = 1;
        instance->m->cur_loc.y = 4;
    }
    else if (side == SECOND)
    {
        instance->m->cur_loc.x = 3;
        instance->m->cur_loc.y = 0;
    }

    int gold_array[GOLD_MOVE_NUM][2] = {
        {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{0,1}
        };

    

    if (side == FIRST){

        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->m->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->m->idle_ability.directions);
    }else if(side == SECOND){
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->m->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->m->idle_ability.directions);

        inverse_vectors(instance->m->ability.directions, GOLD_MOVE_NUM);
        inverse_vectors(instance->m->idle_ability.directions, GOLD_MOVE_NUM);
    }
    
    instance->m->ability.length = GOLD_MOVE_NUM;
    instance->m->idle_ability.length = GOLD_MOVE_NUM;

    instance->m->side = side;

    return instance;
}




