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
    swap_ability(&(this->m->ability),&(this->m->idle_ability));

    PieceKind tmp_kind = this->m->kind;
    this->m->kind = this->m->idle_kind;
    this->m->idle_kind = tmp_kind;

    // display_boardを使わなければこの処理はいらない
    swap_str(this->m->eng_name,this->m->idle_eng_name,12);

   
}



Piece new_piece(){
    Piece piece = malloc(sizeof(*piece));

    piece->m = (struct piece_member*)malloc(sizeof(struct piece_member));
    
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
    piece->clone_piece = clone_piece;
    piece->free_piece = free_piece;


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
bool move(Piece this, Point end, Board board,Referee ref, bool will_promote, Piece *captured){
    for(int i = 0; i < this->m->ability.length; i++){
        Point dest;
        add_vec_to_point(this->m->cur_loc,this->m->ability.directions[i],&dest);
        
        if(is_same_point(dest,end)){
            if(!ref->is_legal_move(ref,board,this,dest)) return false;
            if(will_promote && ((this->get_kind(this) == GOLD) || (this->get_kind(this) == KING))) return false;
            if(will_promote && !ref->can_promote(ref,this,dest)) return false;


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

bool drop(Piece this, Point loc, Board board, Referee ref){

    if(ref->is_legal_drop(ref,board,this,loc)){

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

void set_cur_loc_outside(Piece this){
    this->m->cur_loc.x = -1;
    this->m->cur_loc.y = -1;
}

void promote(Piece this){

    swap_piece_attributes(this);

    this->m->promoted = true;
}

void demote(Piece this){

    swap_piece_attributes(this);

    this->m->promoted = false;

}

Piece clone_piece(Piece this){
    if(this == NULL) return NULL;

    Piece piece = new_piece();
    
    strcpy(piece->m->name,this->m->name);
    strcpy(piece->m->eng_name,this->m->eng_name);
    strcpy(piece->m->idle_eng_name,this->m->idle_eng_name);
    piece->m->kind = this->m->kind;
    piece->m->idle_kind = this->m->idle_kind;
    piece->m->promoted = this->m->promoted;
    piece->m->cur_loc.x = this->m->cur_loc.x;
    piece->m->cur_loc.y = this->m->cur_loc.y;
    piece->m->side = this->m->side;
    copy_ability(&(this->m->ability),&(piece->m->ability));
    copy_ability(&(this->m->idle_ability),&(piece->m->idle_ability));

    return piece;

}

void free_piece(Piece this){
    if(this == NULL) return;

    
    free(this->m);
    free(this);
}

void copy_ability(Ability *original, Ability *dest){
    
    

    for(int i = 0; i < original->length; i++){
        dest->directions[i].x = original->directions[i].x;
        dest->directions[i].y = original->directions[i].y;
    }

    dest->length = original->length;
    

    
}

