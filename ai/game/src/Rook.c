#include "Rook.h"
#include "Piece.h"
#include "Game.h"
#include "Board.h"
#include "utility.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

Piece new_rook(int side){
    Piece instance = new_piece();

    


    strcpy(instance->name, "HI");
    strcpy(instance->eng_name, "ROOK");
    strcpy(instance->idle_eng_name, "DRAGON");

    instance->kind = ROOK;
    instance->idle_kind = DRAGON;

    instance->promoted = false;

    if (side == FIRST)
    {
        instance->cur_loc.x = 4;
        instance->cur_loc.y = 4;
    }
    else if (side == SECOND)
    {
        instance->cur_loc.x = 0;
        instance->cur_loc.y = 0;
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

    convert_array_into_vectors(rook_array, ROOK_MOVE_NUM, instance->ability.directions);
    instance->ability.length = ROOK_MOVE_NUM;

    convert_array_into_vectors(promoted_rook_array, PROMOTED_ROOK_MOVE_NUM, instance->idle_ability.directions);
    instance->idle_ability.length = PROMOTED_ROOK_MOVE_NUM;

    instance->side = side;

    return instance;
}







