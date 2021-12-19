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

   
   

    strcpy(instance->name, "GI");
    strcpy(instance->eng_name, "SILVER");
    strcpy(instance->idle_eng_name, "NARI-SILVER");

    instance->kind = SILVER;
    instance->idle_kind = PROMOTED_SILVER;

    instance->promoted = false;

    if(side == FIRST){
        instance->cur_loc.x = 2;
        instance->cur_loc.y = 4; 
    }else if(side == SECOND){
        instance->cur_loc.x = 2;
        instance->cur_loc.y = 0;
    }


    int silver_array[SILVER_MOVE_NUM][2] = {
        {1,-1},{0,-1},{-1,-1},{-1,1},{1,1}
        };

    int gold_array[GOLD_MOVE_NUM][2] = {
        {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{0,1}
        };




    if(side == FIRST){
        convert_array_into_vectors(silver_array, SILVER_MOVE_NUM, instance->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->idle_ability.directions);

    }else if(side == SECOND){
        convert_array_into_vectors(silver_array, SILVER_MOVE_NUM, instance->ability.directions);
        convert_array_into_vectors(gold_array, GOLD_MOVE_NUM, instance->idle_ability.directions);
        inverse_vectors(instance->ability.directions, SILVER_MOVE_NUM);
        inverse_vectors(instance->idle_ability.directions, GOLD_MOVE_NUM);
    }
    
    instance->ability.length = SILVER_MOVE_NUM;
    instance->idle_ability.length = GOLD_MOVE_NUM;

    instance->side = side;
    
   
    return instance;

}


