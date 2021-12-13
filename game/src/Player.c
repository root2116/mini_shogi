#include "../include/Player.h"
#include "../include/game.h"
#include "../include/Board.h"
#include "../include/Piece.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


Player new_player(int turn){

    
    Player player = calloc(1,sizeof(*player));

    player->move_my_piece = move_my_piece;
    player->drop_my_captured = drop_my_captured;

    player->turn = turn;

    for(int i = 0; i < 10; i++){
        player->captured_pieces[i] = NULL;
    }
    

    return player;
}

//動かせない場合false
static bool move_my_piece(Player this, Move move, Board board){
    
    Piece piece = board->board[move.start.y][move.start.x];
    

    //自分の駒じゃないものは動かせない
    if(piece->get_side(piece) != this->turn) return false;

    
    Piece captured = NULL;
    if(piece->move(piece, move.end, board, move.will_promote,&captured) == false){
        return false;
    }

    if(captured != NULL){
        add_captured(this,captured);
    }

    return true;
    

}

static bool drop_my_captured(Player this, Drop drop, Board board){
    
    Piece piece = pop_captured(this, drop.kind);
    if(piece != NULL){
        piece->drop(piece, drop.loc, board);
        return true;
    }

    return false;
    
    

}

Piece pop_captured(Player this,PieceKind kind){
    for(int i = 0; i < MAX_CAPTURED; i++){
        if(this->captured_pieces[i]->get_kind(this->captured_pieces[i]) == kind){
            Piece res = this->captured_pieces[i];
            this->captured_pieces[i] = NULL;
            return res;
        } 
    }
    return NULL;
}

void add_captured(Player this, Piece piece){

    for(int i = 0; i < MAX_CAPTURED; i++ ){
        if(this->captured_pieces[i] == NULL){
            this->captured_pieces[i] = piece;
            break;
        }
    }
}
