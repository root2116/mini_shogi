
#include "../include/Board.h"
#include "../include/Piece.h"
#include "../include/game.h"
#include "../include/Player.h"

#include "../include/King.h"
#include "../include/Bishop.h"
#include "../include/Rook.h"
#include "../include/Gold.h"
#include "../include/Silver.h"
#include "../include/Pawn.h"

#include "../include/utility.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_on_board(Point p){
    if (p.x >= 0 && p.x <= 4 && p.y >= 0 && p.y <= 4) return true;
    else return false;
}

bool is_on_enemy_area(int side,Point p){
    if(side == FIRST){
        if(p.y == 0) return true;
        else false;
    }else if(side == SECOND){
        if(p.y == 4) return true;
        else false;
    }

    return false;
}


static void display_board(Board this,Player player0, Player player1){
    printf("\n");

    for(int i = 0; i < MAX_CAPTURED; i++){
        Piece captured = player1->captured_pieces[i];
        if(captured != NULL){
            printf("%c|", player1->captured_pieces[i]->get_eng_name(player1->captured_pieces[i])[0] + 32);
        } 
    }

    printf("\n\n");

    for(int i = 0; i < 6; i++){
        
        for(int j = 0; j < 6; j++){
            if(i == 0 && j == 0){
                printf(" |");
            }else if(i == 0 && j != 0){
                printf("%c|", 'A' + (j-1));
            }else if(i != 0 && j == 0){
                printf("%d|",6-i);
            }else{
                Piece piece = this->board[i-1][j-1];

                if(piece !=NULL){
                    if(piece->get_side(piece) == FIRST){
                        printf("%c|", piece->get_eng_name(piece)[0]);
                    }else if(piece->get_side(piece) == SECOND){
                        printf("%c|",piece->get_eng_name(piece)[0]+32);
                    }
                }else{
                    printf(" |");
                }
            }
            
            
        }
        printf("\n");
    }

    printf("\n");

    for(int i = 0; i < MAX_CAPTURED; i++){
        Piece captured = player0->captured_pieces[i];
        if(captured != NULL){
            printf("%c|", player0->captured_pieces[i]->get_eng_name(player0->captured_pieces[i])[0]);
        } 
    }

    printf("\n\n");
}



//ルールはさておき、駒をそこに動かせるか？
static bool can_move(Board this, Piece piece, Point dest){

    //盤面内にないなら動かせない
    if(!is_on_board(dest)) 
        return false;
    
    
    Piece target = this->board[dest.y][dest.x];
    //動く先が味方だったら動かせない
    if(target != NULL){
        if (target->get_side(target) == piece->get_side(piece)) return false;
    }

    //射線上に駒があると動かせない
    Vector move_vec = {dest.x - piece->get_location(piece).x, dest.y - piece->get_location(piece).y};
    int vec_gcd = gcd(abs(move_vec.x),abs(move_vec.y));

    Vector coprime_move_vec = {move_vec.x / vec_gcd, move_vec.y / vec_gcd};

    Point scan = piece->get_location(piece);
    

    Point front = {dest.x - coprime_move_vec.x, dest.y - coprime_move_vec.y};

    while (scan.x != front.x || scan.y != front.y)
    {
        add_vec_to_point(scan, coprime_move_vec, &scan);
        if (this->board[scan.y][scan.x] != NULL)
            return false;

        
    }

    

    if (target == NULL || target->get_side(target) != piece->get_side(piece)){
        
       return true;
    
    }
    

    
 
}

static Piece move_piece(Board this, Piece piece, Point dest){
    Point cur = piece->m->cur_loc;
    this->board[cur.y][cur.x] = NULL;
    Piece captured = this->board[dest.y][dest.x];
    if(captured != NULL){
        captured->betray(captured);
        captured->set_cur_loc_outside(captured);
    }

    this->board[dest.y][dest.x] = piece;

    return captured;


}



static bool can_drop(Board this,Piece piece,Point dest){

    //盤面外には置けない
    if(!is_on_board(dest)) 
        return false;

    
    //相手の陣地に歩は置けない
    if (piece->get_kind(piece) == PAWN && is_on_enemy_area(piece->get_side(piece), dest))
        return false;

    //何も無ければ置ける
    if(this->board[dest.y][dest.x] == NULL)
        return true;
    

    return false;

}

static void drop_piece(Board this,Piece piece, Point dest){

    this->board[dest.y][dest.x] = piece;

}


void clone_board(Board this){

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            Piece piece = this->stack.boards[this->stack.top][i][j];
            if(piece == NULL){
                this->board[i][j] = NULL;
            }else{
                this->board[i][j] = piece->clone_piece(piece);
            }
            
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            Piece piece = this->stack.captured_pieces[this->stack.top][i][j];
            if(piece == NULL){
                this->captured_pieces[i][j] = NULL;
            }else{
                this->captured_pieces[i][j] = piece->clone_piece(piece);
            }
            
        }
    }

}




void free_board(Board this){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(this->board[i][j] == NULL) continue;

            this->board[i][j]->free_piece(this->board[i][j]);
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            if(this->captured_pieces[i][j] == NULL) continue;

            this->captured_pieces[i][j]->free_piece(this->captured_pieces[i][j]);
        }
    }
}

void push_board(Board this){

    if(this->stack.top >= MAX_STACK - 1){
        printf("BoardStack is alreadly full.");
    }else{
        this->stack.top++;
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                this->stack.boards[this->stack.top][i][j] = this->board[i][j];
            }
        }

        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 10; j++){
                this->stack.captured_pieces[this->stack.top][i][j] = this->captured_pieces[i][j];
            }
        }
        
    }
    
    
}

void pop_board(Board this){

    if(this->stack.top <= -1){
        printf("BoardStack is emtpy.");
    }else{
        free_board(this);
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                this->board[i][j] = this->stack.boards[this->stack.top][i][j];
                this->stack.boards[this->stack.top][i][j] = NULL;
            }
        }

        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 10; j++){
                this->captured_pieces[i][j] = this->stack.captured_pieces[this->stack.top][i][j];
                this->stack.captured_pieces[this->stack.top][i][j] = NULL;
            }
        }
        
        this->stack.top--;
    }
    
}



Board new_board()
{
    Board instance = calloc(1,sizeof(*instance));
    instance->display_board = display_board;

    instance->can_move = can_move;
    instance->move_piece = move_piece;
    instance->can_drop = can_drop;
    instance->drop_piece = drop_piece;
    instance->clone_board = clone_board;
    instance->free_board = free_board;
    instance->push_board = push_board;
    instance->pop_board = pop_board;

    

    Piece king0 = new_king(FIRST);
    Piece bishop0 = new_bishop(FIRST);
    Piece rook0 = new_rook(FIRST);
    Piece gold0 = new_gold(FIRST);
    Piece silver0 = new_silver(FIRST);
    Piece pawn0 = new_pawn(FIRST);
    
    Piece king1 = new_king(SECOND);
    Piece bishop1 = new_bishop(SECOND);
    Piece rook1 = new_rook(SECOND);
    Piece gold1 = new_gold(SECOND);
    Piece silver1 = new_silver(SECOND);
    Piece pawn1 = new_pawn(SECOND);

    Piece pieces[PIECE_NUM] = {king0,bishop0,rook0,gold0,silver0,pawn0,king1,bishop1,rook1,gold1,silver1,pawn1};

     //boardとstackを初期化
    instance->stack.top = -1;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            instance->board[i][j] = NULL;
            for(int k = 0; k < MAX_STACK; k++){
                instance->stack.boards[k][i][j] = NULL;
            }
        }
    }  

    

    
    //持ち駒を初期化
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            instance->captured_pieces[i][j] = NULL;
            for(int k = 0; k < MAX_STACK; k++){
                instance->stack.captured_pieces[k][i][j] = NULL;
            }
        }
    }
    


    //駒を配置

    for(int i = 0; i < PIECE_NUM; i++){
        Point location = pieces[i]->get_location(pieces[i]);
        instance->board[location.y][location.x] = pieces[i];
    }

    return instance;
}