#include "Piece.h"
#include "Game.h"
#include "Board.h"
#include "Referee.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>

void swap_ability(Ability *ability1, Ability *ability2){
    int tmp_x,tmp_y;
    for(int i = 0; i < MAX_DIRECTION; i++){
        tmp_x = ability1->directions[i].x;
        ability1->directions[i].x = ability2->directions[i].x;
        ability2->directions[i].x = tmp_x;

        tmp_y = ability1->directions[i].y;
        ability1->directions[i].y = ability2->directions[i].y;
        ability2->directions[i].y = tmp_y;

    }

    int tmp_len = ability1->length;
    ability1->length = ability2->length;
    ability2->length = tmp_len;

}

void swap_piece_attributes(Piece this)
{
    // abilityとidle_abilityをswap.
    swap_ability(&(this->ability),&(this->idle_ability));

    PieceKind tmp_kind = this->kind;
    this->kind = this->idle_kind;
    this->idle_kind = tmp_kind;

    // display_boardを使わなければこの処理はいらない
    swap_str(this->eng_name,this->idle_eng_name,12);

   
}



Piece new_piece(){
    Piece piece = calloc(1,sizeof(struct piece_t));

  
    
    piece->get_location = get_location;
    piece->get_eng_name = get_eng_name;
    piece->get_kind = get_kind;
    piece->get_name = get_name;
    piece->get_side = get_side;


    piece->betray = betray;
    piece->set_cur_loc_outside = set_cur_loc_outside;
    piece->move = move;
    piece->drop = drop;
    piece->promote = promote;
    piece->copy_piece = copy_piece;


    return piece;
}

Point get_location(Piece this){

    return this->cur_loc;

}

int get_side(Piece this){
    
    return this->side;
}

int get_kind(Piece this){

    return this->kind;
}


char* get_name(Piece this){
    
    return this->name;
}

char* get_eng_name(Piece this){

    return this->eng_name;
}




//動かせたらtrue,動かせなかったらfalseが返る
bool move(Piece this, Point end, Board board,Referee ref, bool will_promote, Piece *captured){
    for(int i = 0; i < this->ability.length; i++){
        Point dest;
        add_vec_to_point(this->cur_loc,this->ability.directions[i],&dest);
        
        if(is_same_point(dest,end)){
            if(!ref->is_legal_move(ref,board,this,dest)) return false;
            if(will_promote && ((this->get_kind(this) == GOLD) || (this->get_kind(this) == KING))) return false;
            if(will_promote && !ref->can_promote(ref,this,dest)) return false;


            if(will_promote){
                this->promote(this);
            }

            *captured = board->move_piece(board, this, end);
            this->cur_loc.x = end.x;
            this->cur_loc.y = end.y;
            return true;

        }
    }
    return false;
}

bool drop(Piece this, Point loc, Board board, Referee ref){

    if(ref->is_legal_drop(ref,board,this,loc)){

        this->cur_loc.x = loc.x;
        this->cur_loc.y = loc.y;

        board->drop_piece(board,this,loc);
        return true;
    } 

    return false;
    

}

void betray(Piece this){

    inverse_vectors(this->ability.directions,this->ability.length);
    inverse_vectors(this->idle_ability.directions,this->idle_ability.length);

    if(this->promoted == true){
        demote(this);
    }

    if(this->side == FIRST){
        this->side = SECOND;
    }else if(this->side == SECOND){
        this->side = FIRST;
    }


}

void set_cur_loc_outside(Piece this){
    this->cur_loc.x = -1;
    this->cur_loc.y = -1;
}

void promote(Piece this){

    swap_piece_attributes(this);

    this->promoted = true;
}

void demote(Piece this){

    swap_piece_attributes(this);

    this->promoted = false;

}




void copy_piece(Piece this, Piece copy){
    strcpy(copy->name, this->name);
    strcpy(copy->eng_name, this->eng_name);
    strcpy(copy->idle_eng_name, this->idle_eng_name);
    copy->kind = this->kind;
    copy->idle_kind = this->idle_kind;
    copy->promoted = this->promoted;
    copy->cur_loc.x = this->cur_loc.x;
    copy->cur_loc.y = this->cur_loc.y;
    copy->side = this->side;
    copy_ability(&(this->ability), &(copy->ability));
    copy_ability(&(this->idle_ability), &(copy->idle_ability));
}


void copy_ability(Ability *original, Ability *dest){
    
    

    for(int i = 0; i < original->length; i++){
        dest->directions[i].x = original->directions[i].x;
        dest->directions[i].y = original->directions[i].y;
    }

    dest->length = original->length;
    

    
}

