#include "../include/Silver.h"
#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/utility.h"

#include <stdbool.h>
#include <stdlib.h>



static bool promote(Piece this);


Piece new_silver(int side){
    Piece instance = new_piece();

    instance->move = move;
    

    instance->promote = promote;
   

    instance->m->name = "GI";
    instance->m->eng_name = "SILVER";
    instance->m->kind = SILVER;
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

    Vector *vectors = convert_array_into_vectors(silver_array, SILVER_MOVE_NUM);
    if(side == FIRST){
        
        instance->m->ability.directions = vectors;

    }else if(side == SECOND){

        inverse_vectors(vectors,SILVER_MOVE_NUM);
        instance->m->ability.directions = vectors;
    }
    
    instance->m->ability.length = SILVER_MOVE_NUM;

    instance->m->side = side;
    
   
    return instance;

}



static bool promote(Piece this){
    //もし成れるのなら
    this->m->promoted = true;
    return true;
}



