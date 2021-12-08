#include "../include/Bishop.h"
#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/utility.h"
#include <stdbool.h>
#include <stdlib.h>


static bool promote(Piece this);

Piece new_bishop(int side){
    
    Piece instance = new_piece();

    

    instance->promote = promote;
   
    instance->m->name = "KK";
    
    instance->m->eng_name = "BISHOP";
    instance->m->kind = BISHOP;
    instance->m->promoted = false;

    if (side == FIRST)
    {
        instance->m->cur_loc.x = 3;
        instance->m->cur_loc.y = 4;
    }
    else if (side == SECOND)
    {
        instance->m->cur_loc.x = 1;
        instance->m->cur_loc.y = 0;
    }

    int bishop_array[BISHOP_MOVE_NUM][2] = {
        { 1, 1},{ 2, 2},{ 3, 3},{ 4, 4},
        {-1, 1},{-2, 2},{-3, 3},{-4, 4},
        {-1,-1},{-2,-2},{-3,-3},{-4,-4},
        { 1,-1},{ 2,-2},{ 3,-3},{ 4,-4}
        };
    
    int promoted_bishop_array[PROMOTED_BISHOP_MOVE_NUM][2] = {
        { 1, 1},{ 2, 2},{ 3, 3},{ 4, 4},
        {-1, 1},{-2, 2},{-3, 3},{-4, 4},
        {-1,-1},{-2,-2},{-3,-3},{-4,-4},
        { 1,-1},{ 2,-2},{ 3,-3},{ 4,-4},
        { 1, 0},{ 0, 1},{-1, 0},{0, -1}
        };


    
    instance->m->ability.directions = convert_array_into_vectors(bishop_array,BISHOP_MOVE_NUM);
    instance->m->ability.length = BISHOP_MOVE_NUM;

    instance->m->idle_ability.directions = convert_array_into_vectors(promoted_bishop_array,PROMOTED_BISHOP_MOVE_NUM);
    instance->m->idle_ability.length = PROMOTED_BISHOP_MOVE_NUM;

    instance->m->side = side;

    return instance;
}


static bool promote(Piece this){
    //もし成れるのなら
    this->m->promoted = true;
    return true;
}




