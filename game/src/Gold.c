
#include "../include/Gold.h"
#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/utility.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


Piece new_gold(int side){
    Piece instance = new_piece();

    instance->move = move;
  

    instance->promote = promote;
   

    instance->m->name = calloc(3, sizeof(char));
    strcpy(instance->m->name, "KI");
    instance->m->eng_name = calloc(12, sizeof(char));
    strcpy(instance->m->eng_name, "GOLD");
    instance->m->idle_eng_name = calloc(12, sizeof(char));
    strcpy(instance->m->idle_eng_name, "GOLD");

    instance->m->kind = GOLD;
    instance->m->idle_kind = GOLD;
    
    instance->m->promoted = false;

    if (side == FIRST)
    {
        instance->m->cur_loc.x = 1;
        instance->m->cur_loc.y = 4;
    }
    else if (side == SECOND)
    {
        instance->m->cur_loc.x = 3;
        instance->m->cur_loc.y = 0;
    }

    int gold_array[GOLD_MOVE_NUM][2] = {
        {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{0,1}
        };

    Vector *vectors = convert_array_into_vectors(gold_array, GOLD_MOVE_NUM);
    Vector *idle_vectors = convert_array_into_vectors(gold_array, GOLD_MOVE_NUM);

    if (side == FIRST){

        instance->m->ability.directions = vectors;
        instance->m->idle_ability.directions = idle_vectors;

    }else if(side == SECOND){
        inverse_vectors(vectors,GOLD_MOVE_NUM);
        inverse_vectors(idle_vectors,GOLD_MOVE_NUM);
        instance->m->ability.directions = vectors;
        instance->m->idle_ability.directions = idle_vectors;
    }
    
    instance->m->ability.length = GOLD_MOVE_NUM;
    instance->m->idle_ability.length = GOLD_MOVE_NUM;

    instance->m->side = side;

    return instance;
}




