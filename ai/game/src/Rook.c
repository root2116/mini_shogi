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

    


    strcpy(instance->m->name, "HI");
    strcpy(instance->m->eng_name, "ROOK");
    strcpy(instance->m->idle_eng_name, "DRAGON");

    instance->m->kind = ROOK;
    instance->m->idle_kind = DRAGON;

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

    convert_array_into_vectors(rook_array, ROOK_MOVE_NUM, instance->m->ability.directions);
    instance->m->ability.length = ROOK_MOVE_NUM;

    convert_array_into_vectors(promoted_rook_array, PROMOTED_ROOK_MOVE_NUM, instance->m->idle_ability.directions);
    instance->m->idle_ability.length = PROMOTED_ROOK_MOVE_NUM;

    instance->m->side = side;

    return instance;
}







