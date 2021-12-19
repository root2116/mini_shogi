#include "King.h"
#include "Piece.h"
#include "Game.h"
#include "Board.h"
#include "utility.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Piece new_king(int side){
    Piece instance = new_piece();



    
    strcpy(instance->name, "OU");
    strcpy(instance->eng_name, "KING");
    strcpy(instance->idle_eng_name, "KING");

    instance->kind = KING;
    instance->idle_kind = KING;
    instance->promoted = false;

    if(side == FIRST){
        instance->cur_loc.x = 0;
        instance->cur_loc.y = 4; 
    }else if(side == SECOND){
        instance->cur_loc.x = 4;
        instance->cur_loc.y = 0;
    }

    
    int king_array[KING_MOVE_NUM][2] = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    convert_array_into_vectors(king_array, KING_MOVE_NUM, instance->ability.directions);
    instance->ability.length = KING_MOVE_NUM;

    convert_array_into_vectors(king_array, KING_MOVE_NUM, instance->idle_ability.directions);
    instance->idle_ability.length = KING_MOVE_NUM;
    
    instance->side = side;

    return instance;

}




