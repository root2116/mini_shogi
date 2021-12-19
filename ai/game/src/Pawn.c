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

   
    strcpy(instance->m->name, "FU");
    strcpy(instance->m->eng_name, "PAWN");
    strcpy(instance->m->idle_eng_name, "TO-GOLD");

    instance->m->kind = PAWN;
    instance->m->idle_kind = PROMOTED_PAWN;

    instance->m->promoted = false;

    if (side == FIRST)
    {
        instance->m->cur_loc.x = 0;
        instance->m->cur_loc.y = 3;
    }
    else if (side == SECOND)
    {
        instance->m->cur_loc.x = 4;
        instance->m->cur_loc.y = 1;
    }

    int pawn_array[PAWN_MOVE_NUM][2] = {
        {0,-1}
    };

    int gold_array[GOLD_MOVE_NUM][2] = {
        {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{0,1}
        };


   
    if(side == FIRST){
        convert_array_into_vectors(pawn_array, PAWN_MOVE_NUM, instance->m->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->m->idle_ability.directions);
    }else if(side == SECOND){
        convert_array_into_vectors(pawn_array, PAWN_MOVE_NUM, instance->m->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->m->idle_ability.directions);
        inverse_vectors(instance->m->ability.directions, PAWN_MOVE_NUM);
        inverse_vectors(instance->m->idle_ability.directions, GOLD_MOVE_NUM);

    }


    
    instance->m->ability.length = PAWN_MOVE_NUM;
    instance->m->idle_ability.length = GOLD_MOVE_NUM;

    instance->m->side = side;

    return instance;
}




