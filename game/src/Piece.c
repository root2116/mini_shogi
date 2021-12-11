#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/utility.h"
#include <stdlib.h>

void swap_piece_attributes(Piece this)
{
    // abilityとidle_abilityをswap.
    Ability tmp_ability = this->m->ability;
    this->m->ability = this->m->idle_ability;
    this->m->idle_ability = tmp_ability;

    PieceKind tmp_kind = this->m->kind;
    this->m->kind = this->m->idle_kind;
    this->m->idle_kind = tmp_kind;

    // display_boardを使わなければこの処理はいらない
    char *tmp_name = this->m->eng_name;
    this->m->eng_name = this->m->idle_eng_name;
    this->m->idle_eng_name = tmp_name;
}

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
    piece->promote = promote;


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




//動かせたらtrue,動かせなかったらfalseが返る
bool move(Piece this, Point end, Board board, bool will_promote, Piece *captured){
    for(int i = 0; i < this->m->ability.length; i++){
        Point dest;
        add_vec_to_point(this->m->cur_loc,this->m->ability.directions[i],&dest);
        
        if(is_same_point(dest,end)){
            if(!board->can_move(board,this,dest)) return false;
            if(will_promote && !board->can_promote(board,this,dest)) return false;


            if(will_promote){
                this->promote(this);
            }

            *captured = board->move_piece(board, this, end);
            this->m->cur_loc.x = end.x;
            this->m->cur_loc.y = end.y;
            return true;

        }
    }
    return false;
}

bool drop(Piece this, Point loc, Board board){

    if(board->can_drop(board,this,loc)){

        this->m->cur_loc.x = loc.x;
        this->m->cur_loc.y = loc.y;

        board->drop_piece(board,this,loc);
        return true;
    } 

    return false;
    

}

void betray(Piece this){

    inverse_vectors(this->m->ability.directions,this->m->ability.length);
    inverse_vectors(this->m->idle_ability.directions,this->m->idle_ability.length);

    if(this->m->promoted == true){
        demote(this);
    }

    if(this->m->side == FIRST){
        this->m->side = SECOND;
    }else if(this->m->side == SECOND){
        this->m->side = FIRST;
    }


}

void promote(Piece this){

    swap_piece_attributes(this);

    this->m->promoted = true;
}

void demote(Piece this){

    swap_piece_attributes(this);

    this->m->promoted = false;

}

