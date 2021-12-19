#include "Player.h"
#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include "Referee.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


Player new_player(int turn){

    
    Player player = malloc(sizeof(*player));

    player->move_my_piece = move_my_piece;
    player->drop_my_captured = drop_my_captured;

    player->turn = turn;
    


    return player;
}

//動かせない場合false
static bool move_my_piece(Player this, Move move, Board board, Referee ref){
    
    Piece piece = board->board[move.start.y][move.start.x];
    
    //存在しない駒は動かせない
    if(piece == NULL) return false;

    //自分の駒じゃないものは動かせない
    if(piece->get_side(piece) != this->turn) return false;

    //歩は成れるなら必ず成る
    if(piece->get_kind(piece) == PAWN && ref->can_promote(ref,piece,move.end) && move.will_promote == false) return false;


    Piece captured = NULL;
    if(piece->move(piece, move.end, board,ref, move.will_promote,&captured) == false){
        return false;
    }

    if(captured != NULL){
        add_captured(this,captured);
    }

    return true;
    

}

static bool drop_my_captured(Player this, Drop drop, Board board, Referee ref){
    
    Piece piece = get_captured(this, drop.kind);
    if(piece != NULL){
        if(piece->drop(piece, drop.loc, board, ref)){
            remove_captured(this,piece->get_kind(piece));
            return true;
        }
        
    }

    return false;
    
    

}

Piece pop_captured(Player this,PieceKind kind){
    for(int i = 0; i < MAX_CAPTURED; i++){
        Piece piece = this->captured_pieces[i];
        if(piece == NULL) continue;

        if(piece->get_kind(piece) == kind){
            Piece res = piece;
            this->captured_pieces[i] = NULL;
            return res;
        } 
    }
    return NULL;
}

Piece get_captured(Player this,PieceKind kind){
    for(int i = 0; i < MAX_CAPTURED; i++){
        Piece piece = this->captured_pieces[i];
        if(piece == NULL) continue;
        if(piece->get_kind(piece) == kind){
            Piece res = piece;
            return res;
        } 
    }
    return NULL;
}

void remove_captured(Player this, PieceKind kind){
    for(int i = 0; i < MAX_CAPTURED; i++){
        Piece piece = this->captured_pieces[i];
        if(piece == NULL) continue;

        if(piece->get_kind(piece) == kind){
            this->captured_pieces[i] = NULL;
            break;
        } 
    }
}
void add_captured(Player this, Piece piece){

    for(int i = 0; i < MAX_CAPTURED; i++ ){
        if(this->captured_pieces[i] == NULL){
            this->captured_pieces[i] = piece;
            break;
        }
    }
}
