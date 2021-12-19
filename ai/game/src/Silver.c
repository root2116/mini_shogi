#include "Silver.h"
#include "Piece.h"
#include "Game.h"
#include "Board.h"
#include "utility.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

Piece new_silver(int side){
    Piece instance = new_piece();

   
   

    strcpy(instance->m->name, "GI");
    strcpy(instance->m->eng_name, "SILVER");
    strcpy(instance->m->idle_eng_name, "NARI-SILVER");

    instance->m->kind = SILVER;
    instance->m->idle_kind = PROMOTED_SILVER;

    instance->m->promoted = false;

    if(side == FIRST){
        instance->m->cur_loc.x = 2;
        instance->m->cur_loc.y = 4; 
    }else if(side == SECOND){
        instance->m->cur_loc.x = 2;
        instance->m->cur_loc.y = 0;
    }


    int silver_array[SILVER_MOVE_NUM][2] = {
        {1,-1},{0,-1},{-1,-1},{-1,1},{1,1}
        };

    int gold_array[GOLD_MOVE_NUM][2] = {
        {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{0,1}
        };




    if(side == FIRST){
        convert_array_into_vectors(silver_array, SILVER_MOVE_NUM, instance->m->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->m->idle_ability.directions);

    }else if(side == SECOND){
        convert_array_into_vectors(silver_array, SILVER_MOVE_NUM, instance->m->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->m->idle_ability.directions);
        inverse_vectors(instance->m->ability.directions, SILVER_MOVE_NUM);
        inverse_vectors(instance->m->idle_ability.directions, GOLD_MOVE_NUM);
    }
    
    instance->m->ability.length = SILVER_MOVE_NUM;
    instance->m->idle_ability.length = GOLD_MOVE_NUM;

    instance->m->side = side;
    
   
    return instance;

}


