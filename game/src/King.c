#include "../include/King.h"
#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/utility.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Piece new_king(int side){
    Piece instance = new_piece();

    instance->promote = promote;


    instance->m->name = calloc(3, sizeof(char));
    strcpy(instance->m->name, "OU");
    instance->m->eng_name = calloc(12, sizeof(char));
    strcpy(instance->m->eng_name, "KING");
    instance->m->idle_eng_name = calloc(12, sizeof(char));
    strcpy(instance->m->idle_eng_name, "KING");

    instance->m->kind = KING;
    instance->m->idle_kind = KING;
    instance->m->promoted = false;

    if(side == FIRST){
        instance->m->cur_loc.x = 0;
        instance->m->cur_loc.y = 4; 
    }else if(side == SECOND){
        instance->m->cur_loc.x = 4;
        instance->m->cur_loc.y = 0;
    }

    
    int king_array[KING_MOVE_NUM][2] = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    instance->m->ability.directions = convert_array_into_vectors(king_array,KING_MOVE_NUM);
    instance->m->ability.length = KING_MOVE_NUM;

    instance->m->idle_ability.directions = convert_array_into_vectors(king_array, KING_MOVE_NUM);
    instance->m->ability.length = KING_MOVE_NUM;
    
    instance->m->side = side;

    return instance;

}




