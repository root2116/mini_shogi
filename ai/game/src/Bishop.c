#include "Bishop.h"
#include "Piece.h"
#include "Game.h"
#include "Board.h"
#include "utility.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>




Piece new_bishop(int side){
    
    Piece instance = new_piece();

    
    strcpy(instance->name,"KK"); 
    strcpy(instance->eng_name,"BISHOP");
    strcpy(instance->idle_eng_name,"HORSE");
    

    instance->kind = BISHOP;
    instance->idle_kind = HORSE;

    instance->promoted = false;

    if (side == FIRST)
    {
        instance->cur_loc.x = 3;
        instance->cur_loc.y = 4;
    }
    else if (side == SECOND)
    {
        instance->cur_loc.x = 1;
        instance->cur_loc.y = 0;
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

    convert_array_into_vectors(bishop_array, BISHOP_MOVE_NUM, instance->ability.directions);
    instance->ability.length = BISHOP_MOVE_NUM;

    convert_array_into_vectors(promoted_bishop_array, PROMOTED_BISHOP_MOVE_NUM, instance->idle_ability.directions);
    instance->idle_ability.length = PROMOTED_BISHOP_MOVE_NUM;

    instance->side = side;

    return instance;
}







