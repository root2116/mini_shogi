#include "Pawn.h"
#include "Piece.h"
#include "Game.h"
#include "Board.h"
#include "utility.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

Piece new_pawn(int side){
    Piece instance = new_piece();

   
    strcpy(instance->name, "FU");
    strcpy(instance->eng_name, "PAWN");
    strcpy(instance->idle_eng_name, "TO-GOLD");

    instance->kind = PAWN;
    instance->idle_kind = PROMOTED_PAWN;

    instance->promoted = false;

    if (side == FIRST)
    {
        instance->cur_loc.x = 0;
        instance->cur_loc.y = 3;
    }
    else if (side == SECOND)
    {
        instance->cur_loc.x = 4;
        instance->cur_loc.y = 1;
    }

    int pawn_array[PAWN_MOVE_NUM][2] = {
        {0,-1}
    };

    int gold_array[GOLD_MOVE_NUM][2] = {
        {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{0,1}
        };


   
    if(side == FIRST){
        convert_array_into_vectors(pawn_array, PAWN_MOVE_NUM, instance->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->idle_ability.directions);
    }else if(side == SECOND){
        convert_array_into_vectors(pawn_array, PAWN_MOVE_NUM, instance->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->idle_ability.directions);
        inverse_vectors(instance->ability.directions, PAWN_MOVE_NUM);
        inverse_vectors(instance->idle_ability.directions, GOLD_MOVE_NUM);

    }


    
    instance->ability.length = PAWN_MOVE_NUM;
    instance->idle_ability.length = GOLD_MOVE_NUM;

    instance->side = side;

    return instance;
}




