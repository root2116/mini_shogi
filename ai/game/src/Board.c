
#include "Board.h"
#include "Piece.h"
#include "Game.h"
#include "Player.h"

#include "King.h"
#include "Bishop.h"
#include "Rook.h"
#include "Gold.h"
#include "Silver.h"
#include "Pawn.h"

#include "utility.h"


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

Piece find_clone_piece_from_board(Board this, Piece piece){

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            Piece target = this->board[i][j];
            if (target == NULL)
                continue;
            if (is_same_point(target->get_location(target), piece->get_location(piece))){
                return target;
            }
        }
    }

    return NULL;
}

Piece pop_clone_piece_from_captured_pieces(Board this, Piece piece){
    for(int i = 0; i < 10; i++){
        int side = piece->get_side(piece);
        Piece copy = this->captured_pieces[side][i];
        if(copy == NULL) continue;

        if(copy->get_kind(copy) == piece->get_kind(piece)){
            this->captured_pieces[side][i] = NULL;
            return copy;
        }
    }

    return NULL;
}

void add_piece_to_captured_pieces(Board this, Piece piece){
    for(int i = 0; i < 10; i++){
        if(piece == NULL) continue;

        int side = piece->get_side(piece);
        if(this->captured_pieces[side][i] == NULL){
            this->captured_pieces[side][i] = piece;
            return;
        }
    }
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

    fprintf(stderr,"Something went wrong in can_move()\n");
    return false;

    
 
}

static Piece move_piece(Board this, Piece piece, Point dest){
    Point cur = piece->cur_loc;
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


void create_next_board(Board this, Piece piece, Point dest){
    this->push_pieces(this);
    this->swap_board(this);

    Piece piece_copy = find_clone_piece_from_board(this,piece);
    if (piece_copy != NULL){
        Piece captured = this->move_piece(this, piece_copy, dest);
        add_piece_to_captured_pieces(this,captured);
    }else{
        piece_copy = pop_clone_piece_from_captured_pieces(this, piece);
        if(piece_copy == NULL){
            fprintf(stderr, "Not found captured_pieces.\n");
        }
        this->drop_piece(this, piece_copy, dest);
    }

    piece_copy->cur_loc.x = dest.x;
    piece_copy->cur_loc.y = dest.y;
}





//boardにあるpieceを解放してNULLを入れる
void free_pieces(Board this){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(this->board[i][j] == NULL) continue;

            free(this->board[i][j]);
            this->board[i][j] = NULL;
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            if(this->captured_pieces[i][j] == NULL) continue;

            free(this->captured_pieces[i][j]);
            this->captured_pieces[i][j] = NULL;
        }
    }

    
}

//スタックに現在のboardとcaptured_pieces内の駒をコピーする
void push_pieces(Board this){

    Piece pieces[PIECE_NUM];
    if(this->stack.top >= MAX_STACK - 1){
        printf("BoardStack is alreadly full.");
    }else{
        this->stack.top++;

        int count1 = 0;
        

        //スタックからピースを集める
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                if (this->stack.boards[this->stack.top][i][j] != NULL){
                    pieces[count1] = this->stack.boards[this->stack.top][i][j];
                    count1++;
                }
            }
        }

        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 10; j++){
                if(this->stack.captured_pieces[this->stack.top][i][j] != NULL){
                    pieces[count1] = this->stack.captured_pieces[this->stack.top][i][j];
                    count1++;
                }
            }
        }


        //スタックをクリアする
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                this->stack.boards[this->stack.top][i][j] = NULL;
            }
        }

        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 10; j++){
                this->stack.captured_pieces[this->stack.top][i][j] = NULL;
            }
        }

        

        
        int count2 = 0;
        //スタックのピースに情報をコピーして配置をboardと同じにする

        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                if(this->board[i][j] != NULL){
                    this->board[i][j]->copy_piece(this->board[i][j], pieces[count2]);
                    this->stack.boards[this->stack.top][i][j] = pieces[count2];
                    count2++;
                }else{
                    this->stack.boards[this->stack.top][i][j] = NULL;
                }
            }
        }

        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 10; j++){
                if(this->captured_pieces[i][j] != NULL){
                    this->captured_pieces[i][j]->copy_piece(this->captured_pieces[i][j], pieces[count2]);
                    this->stack.captured_pieces[this->stack.top][i][j] = pieces[count2];
                    count2++;
                }
                
            }
        }
        
    }
    
    
}

void swap_board(Board this){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            Piece tmp = this->board[i][j];
            this->board[i][j] = this->stack.boards[this->stack.top][i][j];
            this->stack.boards[this->stack.top][i][j] = tmp;
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            Piece tmp = this->captured_pieces[i][j];
            this->captured_pieces[i][j] = this->stack.captured_pieces[this->stack.top][i][j];
            this->stack.captured_pieces[this->stack.top][i][j] = tmp;
        }
    }

}

//スタックからボードを取り出して、boardに反映
void restore_board(Board this){

    Piece pieces[12];
    int count1 = 0;

    if(this->stack.top <= -1){
        printf("BoardStack is emtpy.");
    }else{
        
        this->swap_board(this);
        this->stack.top--;
    }
    
}



void free_board(Board this){
    this->free_pieces(this);
}

void copy_board(Board this, Board copy){
    Piece pieces[12];
    int count1 = 0;


    //ピースを集める
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(copy->board[i][j] != NULL){
                pieces[count1] = copy->board[i][j];
                count1++;
            }
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            if(copy->captured_pieces[i][j] != NULL){
                pieces[count1] = copy->captured_pieces[i][j];
                count1++;
            }
        }
    }

    //copyをクリアする
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            copy->board[i][j] = NULL;
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            copy->captured_pieces[i][j] = NULL;
        }
    }

    
    int count2 = 0;
    //コピーする
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(this->board[i][j] != NULL){
                Piece piece_copy = pieces[count2];
                Piece original = this->board[i][j];
                original->copy_piece(original,piece_copy);
                copy->board[i][j] = piece_copy;
                count2++;
            }else{
                copy->board[i][j] = NULL;
            }
            
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            if(this->captured_pieces[i][j] != NULL){
                Piece piece_copy = pieces[count2];
                Piece original = this->captured_pieces[i][j];
                original->copy_piece(original,piece_copy);
                copy->captured_pieces[i][j] = piece_copy;
                count2++;
            }else{
                copy->captured_pieces[i][j] = NULL;
            }
        }
    }
}


Board new_board()
{
    Board instance = malloc(sizeof(*instance));
    instance->display_board = display_board;

    instance->can_move = can_move;
    instance->move_piece = move_piece;
    instance->can_drop = can_drop;
    instance->drop_piece = drop_piece;
    instance->create_next_board = create_next_board;
    instance->free_pieces = free_pieces;
    instance->push_pieces = push_pieces;
    instance->restore_board = restore_board;
    instance->free_board = free_board;
    instance->copy_board = copy_board;
    instance->swap_board = swap_board;

    

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
    

    //スタックに駒を12個入れる
    for (int k = 0; k < MAX_STACK; k++){
        int count = 0;
        while(count < 12){
            instance->stack.boards[k][count / 5][count % 5] = new_piece();
            count++;
        }
    }

   


    //駒を配置
    for(int i = 0; i < PIECE_NUM; i++){
        Point location = pieces[i]->get_location(pieces[i]);
        instance->board[location.y][location.x] = pieces[i];
    }

    return instance;
}