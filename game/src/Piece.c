#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/utility.h"
#include <stdlib.h>






Piece new_piece(){
    Piece piece = calloc(1,sizeof(*piece));

    piece->m = (struct piece_member*)calloc(1, sizeof(struct piece_member));
    
    piece->get_location = get_location;
    piece->get_eng_name = get_eng_name;
    piece->get_kind = get_kind;
    piece->get_name = get_name;
    piece->get_side = get_side;

    piece->betray = betray;
    piece->move = move;
    piece->drop = drop;


    return piece;
}

Point get_location(Piece this){

    return this->m->cur_loc;

}

int get_side(Piece this){
    
    return this->m->side;
}

int get_kind(Piece this){

    return this->m->kind;
}


char* get_name(Piece this){
    
    return this->m->name;
}

char* get_eng_name(Piece this){

    return this->m->eng_name;
}




//駒を取ったらそれを返し、取らなかったらNULLを返す。そもそも動かせなかったら自分自身を返す
Piece move(Piece this, Point end, Board board){
    for(int i = 0; i < this->m->ability.length; i++){
        Point dest;
        add_vec_to_point(this->m->cur_loc,this->m->ability.directions[i],&dest);
        
        if(is_same_point(dest,end)){
            if(board->can_move(board,this,dest)){
                
                Piece captured = board->move_piece(board,this,end);
                this->m->cur_loc.x = end.x;
                this->m->cur_loc.y = end.y;
                return captured;
            }else{
                return this;
            }
        }
    }
    return this;
}

bool drop(Piece this, Point loc, Board board){

    if(board->can_drop(board,loc)){

        this->m->cur_loc.x = loc.x;
        this->m->cur_loc.y = loc.y;

        board->drop_piece(board,this,loc);
        return true;
    } 

    return false;
    

}

void betray(Piece this){

    inverse_vectors(this->m->ability.directions,this->m->ability.length);

    if(this->m->side == FIRST){
        this->m->side = SECOND;
    }else if(this->m->side == SECOND){
        this->m->side = FIRST;
    }


}

void promote(Piece this){
    //abilityとidle_abilityをswap. eng_nameとidle_eng_nameもswap.
}
