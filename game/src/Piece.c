#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/utility.h"
#include <stdlib.h>
#include <string.h>

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
bool move(Piece this, Point end, Board board, bool will_promote, Piece *captured){
    for(int i = 0; i < this->m->ability.length; i++){
        Point dest;
        add_vec_to_point(this->m->cur_loc,this->m->ability.directions[i],&dest);
        
        if(is_same_point(dest,end)){
            if(!board->is_legal_move(board,this,dest)) return false;
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

Piece clone_piece(Piece this){
    if(this == NULL) return NULL;

    Piece piece = new_piece();
    piece->m->name = calloc(3,sizeof(char));
    piece->m->eng_name = calloc(12,sizeof(char));
    piece->m->idle_eng_name = calloc(12,sizeof(char));
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

    free(this->m->name);
    free(this->m->eng_name);
    free(this->m->idle_eng_name);
    free(this->m->ability.directions);
    free(this->m->idle_ability.directions);
}

void copy_ability(Ability *original, Ability *dest){
    
    Vector *directions = calloc(original->length,sizeof(Vector));

    for(int i = 0; i < original->length; i++){
        directions[i].x = original->directions[i].x;
        directions[i].y = original->directions[i].y;
    }

    dest->length = original->length;
    dest->directions = directions;

    
}

