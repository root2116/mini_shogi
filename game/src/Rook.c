#include "../include/Rook.h"
#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/utility.h"

#include <stdbool.h>
#include <stdlib.h>




Piece new_rook(int side){
    Piece instance = new_piece();

    instance->move = move;
   

    instance->promote = promote;
    

    instance->m->name = "HI";
    instance->m->eng_name = "ROOK";
    instance->m->idle_eng_name = "DRAGON";

    instance->m->kind = ROOK;
    instance->m->promoted = false;

    if (side == FIRST)
    {
        instance->m->cur_loc.x = 4;
        instance->m->cur_loc.y = 4;
    }
    else if (side == SECOND)
    {
        instance->m->cur_loc.x = 0;
        instance->m->cur_loc.y = 0;
    }

    int rook_array[ROOK_MOVE_NUM][2] = {
        { 1, 0},{ 2, 0},{ 3, 0},{ 4, 0},
        { 0, 1},{ 0, 2},{ 0, 3},{ 0, 4},
        {-1, 0},{-2, 0},{-3, 0},{-4, 0},
        { 0,-1},{ 0,-2},{ 0,-3},{ 0,-4}
        };

    int promoted_rook_array[PROMOTED_ROOK_MOVE_NUM][2] = {
        { 1, 0},{ 2, 0},{ 3, 0},{ 4, 0},
        { 0, 1},{ 0, 2},{ 0, 3},{ 0, 4},
        {-1, 0},{-2, 0},{-3, 0},{-4, 0},
        { 0,-1},{ 0,-2},{ 0,-3},{ 0,-4},
        { 1,-1},{ 1, 1},{-1, 1},{-1,-1}
    };


    instance->m->ability.directions = convert_array_into_vectors(rook_array,ROOK_MOVE_NUM);
    instance->m->ability.length = ROOK_MOVE_NUM;

    instance->m->idle_ability.directions = convert_array_into_vectors(promoted_rook_array,PROMOTED_ROOK_MOVE_NUM);
    instance->m->idle_ability.length = PROMOTED_ROOK_MOVE_NUM;

    instance->m->side = side;

    return instance;
}







