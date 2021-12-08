#include "../include/Pawn.h"
#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/utility.h"

#include <stdbool.h>
#include <stdlib.h>


static bool promote(Piece this);

Piece new_pawn(int side){
    Piece instance = new_piece();

    instance->move = move;

    instance->promote = promote;
    
    instance->m->name = "FU";
    instance->m->eng_name = "PAWN";
    instance->m->kind = PAWN;
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


    Vector *vectors = convert_array_into_vectors(pawn_array, PAWN_MOVE_NUM);
    Vector *idle_vectors = convert_array_into_vectors(gold_array, GOLD_MOVE_NUM);

    if(side == FIRST){
        instance->m->ability.directions = vectors;
        instance->m->idle_ability.directions = idle_vectors;

    }else if(side == SECOND){
        inverse_vectors(vectors,PAWN_MOVE_NUM);
        inverse_vectors(idle_vectors,GOLD_MOVE_NUM);

        instance->m->ability.directions = vectors;
        instance->m->idle_ability.directions = idle_vectors;
    }


    
    instance->m->ability.length = PAWN_MOVE_NUM;
    instance->m->idle_ability.length = GOLD_MOVE_NUM;

    instance->m->side = side;

    return instance;
}


static bool promote(Piece this){
    //もし成れるのなら
    this->m->promoted = true;
    return true;
}


