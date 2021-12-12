
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

//王手を回避する手かを判定する
bool is_evasive_move(Board this, Piece piece, Point dest)
{

    this->clone_board(this);

    this->move_piece(this, piece, dest);

    piece->m->cur_loc.x = dest.x;
    piece->m->cur_loc.y = dest.y;

    if (this->judge_check(this,piece->get_side(piece)))
    {
        this->restore_board(this);
        return false;
    }
    else
    {
        this->record_board(this);
        return true;
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

static void update_turn(Board this){
    if(this->turn == FIRST) this->turn = SECOND;
    else if(this->turn == SECOND) this->turn = FIRST;
}

static void increment_turn_count(Board this){
    this->turn_count++;
}

static int get_turn(Board this){
    return this->turn;
}

static int get_turn_count(Board this){
    return this->turn_count;
}

static bool can_move(Board this, Piece piece, Point dest){

    //盤面内にないなら動かせない
    if(!is_on_board(dest)) 
        return false;
    
    


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

    Piece target = this->board[dest.y][dest.x];
    //動く先が味方だったら動かせない
    if(target != NULL){
        if (target->get_side(target) == piece->get_side(piece)) return false;
    }

    if (target == NULL || target->get_side(target) != piece->get_side(piece)){
        if(this->checked[piece->get_side(piece)]){
            if(is_evasive_move(this,piece,dest)){
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
    }
    

    
 
}

static Piece move_piece(Board this, Piece piece, Point dest){
    Point cur = piece->m->cur_loc;
    this->board[cur.y][cur.x] = NULL;
    Piece captured = this->board[dest.y][dest.x];
    if(captured != NULL){
        captured->betray(captured);
    }

    this->board[dest.y][dest.x] = piece;

    return captured;


}

static bool can_drop(Board this,Piece piece,Point dest){

    //盤面外には置けない
    if(!is_on_board(dest)) 
        return false;

    //二歩なら置けない
    if(this->check_double_pawn(this,piece,dest)) 
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


static bool can_promote(Board this, Piece piece, Point dest, Move move){
    //成れるかどうか判定する
    if (move.will_promote){//dest[2] == "N";最後がN
        if (get_location(piece).y >= 0 && get_location(piece).y <= 4){//直接指してない
            if((dest.y >= 0 && dest.y <= 1) || (get_location(piece).y >= 0 && get_location(piece).y <= 1)) { //4A-5Aに移動||4A-5Aから移動
                return true; //成る
        }
        else
            return false; //反則;敗北

    }
    
}

}

//現在の盤面を文字列に変えて、historyに保存する
static void record_board(Board this){
   
}

//千日手を判定する
static bool check_repetition(Board this){
    
}

//side側が王手をかけられているか判定する
static bool judge_check(Board this,int side){
    //sideの王の位置を探す
    bool flag1 = false;
    Point kingLoc;
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            Piece piece1 = this->board[i][j];
            if(piece1 != NULL){
                if(piece1->get_kind(piece1) == KING && piece1->get_side(piece1) == side){
                    kingLoc = piece1->get_location(piece1);
                    flag1 = true;
                    break;
                }
            }
        }
        if(flag1) break;
    }
    //相手側の駒の動ける範囲に自分の王が含まれているか
    bool finalFlag = false;
    bool flag2 = false;
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            Piece piece2 = this->board[i][j];
            if(piece2 != NULL){
                if(piece2->get_side(piece2) == 1 - side){
                    for(int k = 0; k < piece2->m->ability.length; k++){
                        Point dest;
                        add_vec_to_point(piece2->m->cur_loc,piece2->m->ability.directions[k],&dest);
                        if(is_same_point(dest,kingLoc)){
                            if(this->can_move(this,piece2,dest)){
                                finalFlag = true;
                                flag2 = true;
                                break;
                            }
                        }
                    }
                    if(flag2) break;
                }
            }
        }
        if(flag2) break;
    }

    if(finalFlag){
        this->checked[side] = true;
    }else{
        this->checked[side] = false;
    }

    return finalFlag;
}

//歩が2個しかないことを仮定している二歩判定
bool check_double_pawn(Board this, Piece piece, Point dest){

    //そもそも歩じゃなかったら二歩にはならない
    if(piece->get_kind(piece) != PAWN) return false;


    Piece pawn = NULL;
    
    for(int i = 0;i< 5; i++){
        for(int j = 0; j < 5; j++){
            Piece target = this->board[i][j];
            if(target == NULL) continue;

            if (target->get_kind(target) == PAWN){
                pawn = target;
            }
        }
    }

    //盤上に歩が無ければ二歩にはならない
    if(pawn == NULL) return false;


    int side0 = piece->get_side(piece);
    int side1 = pawn->get_side(pawn);
    int x_0 = dest.x;
    int x_1 = pawn->get_location(pawn).x;
   
    if(side0 == side1 && x_0 == x_1) return true;
    else return false;

}



void clone_board(Board this){

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            this->board_copy[i][j] = clone_piece(this->board[i][j]);
        }
    }

}

void restore_board(Board this){

    this->free_board(this);
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            
            this->board[i][j] = this->board_copy[i][j];
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
}



Board new_board(int turn)
{
    Board instance = calloc(1,sizeof(*instance));
    instance->turn = turn;
    instance->turn_count = 1;
    instance->checked[0] = false;
    instance->checked[1] = false;

    instance->display_board = display_board;
    instance->update_turn = update_turn;
    instance->increment_turn_count = increment_turn_count;
    instance->get_turn = get_turn;
    instance->get_turn_count = get_turn_count;
    instance->can_move = can_move;
    instance->move_piece = move_piece;
    instance->can_drop = can_drop;
    instance->drop_piece = drop_piece;
    instance->can_promote = can_promote;
    instance->record_board = record_board;
    instance->check_repetition = check_repetition;
    instance->judge_check = judge_check;
    instance->check_double_pawn = check_double_pawn;
    instance->clone_board = clone_board;
    instance->restore_board = restore_board;
    instance->free_board = free_board;
    

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

     //boardを初期化
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            instance->board[i][j] = NULL;
            instance->board_copy[i][j] = NULL;
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 10; j++){
            instance->captured_pieces[i][j] = NULL;
        }
    }
    
    

    //historyを初期化
    for(int i = 0; i < 150; i++){
        for(int j = 0; j < 25; j++){
            instance->history[i][j] = '\0';
        }
    }

    //駒を配置

    for(int i = 0; i < PIECE_NUM; i++){
        Point location = pieces[i]->get_location(pieces[i]);
        instance->board[location.y][location.x] = pieces[i];
    }

    return instance;
}