#include "../include/Referee.h"
#include "../include/Piece.h"
#include "../include/Board.h"
#include "../include/list.h"

#include "../include/utility.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

Piece find_clone_piece(Board this, Piece piece){
    
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            Piece target = this->board[i][j];
            if(target == NULL) continue;
            if(is_same_point(target->get_location(target),piece->get_location(piece))){
                return target;
            }
        }
    }

    return NULL;
    
    
}

//次の盤面で王手をかけられるならtrue,かけられないならfalse
bool will_be_checked(Referee this, Board board, Piece piece, Point dest){
    board->copy_board(board);
    board->clone_board(board);

    Piece clone_piece = find_clone_piece(board,piece);
    if(clone_piece == NULL){
        clone_piece = piece->clone_piece(piece);
        board->drop_piece(board,clone_piece,dest);
    }else{
        board->move_piece(board, clone_piece, dest);
    }
    

    clone_piece->m->cur_loc.x = dest.x;
    clone_piece->m->cur_loc.y = dest.y;

    if (this->judge_check(this,board, clone_piece->get_side(clone_piece)))
    {
        
        board->restore_board(board);
        return true;
    }
    else
    {
        board->restore_board(board);
        return false;
    }
}



static void display_history(Referee this)
{
    for (int i = 0; i <= this->get_turn_count(this); i++)
    {
        printf("%s\n", this->history[i]);
    }
}

static void update_turn(Referee this){
    if(this->turn == FIRST) this->turn = SECOND;
    else if(this->turn == SECOND) this->turn = FIRST;
}

static void increment_turn_count(Referee this){
    this->turn_count++;
}

static int get_turn(Referee this){
    return this->turn;
}

static int get_turn_count(Referee this){
    return this->turn_count;
}

//合法手か？
static bool is_legal_move(Referee this, Board board, Piece piece, Point dest){
    if(board->can_move(board,piece,dest) == false) return false;

    if(this->will_be_checked(this,board,piece,dest)) return false;
    else return true;
}


static bool is_legal_drop(Referee this, Board board, Piece piece, Point dest){
    if(board->can_drop(board,piece,dest) == false) return false;

    //二歩なら置けない
    if (this->check_double_pawn(this,board, piece, dest)) return false;

    
    if(this->will_be_checked(this,board,piece,dest)) return false;
    
    
    return true;

}

static bool can_promote(Referee this, Piece piece, Point dest){
    //成れるかどうか判定する
    if (piece->get_side(piece) == FIRST){ //PieceがFistなら
        if(dest.y == 0 || piece->get_location(piece).y == 0){ //敵陣に移動or敵陣から移動
            return true; //成る
        }
        else
            return false;

    }
    else if (piece->get_side(piece) == SECOND){ //PieceがFistなら
        if(dest.y == 4 || piece->get_location(piece).y == 4) { //敵陣に移動or敵陣から移動
            return true; //成る
        }
        else
            return false;
    }
     //反則;敗北

}

//現在の盤面を文字列に変えて、history[turn_count]に保存する
//手持ちのコマはplayer0, 1の順でソートして格納
static void record_board(Referee this, Board board){
    
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            
            Piece piece = board->board[i][j];

            if(piece !=NULL){
                if(piece->get_side(piece) == FIRST){
                    this->history[this->turn_count][5 * i + j] = piece->get_eng_name(piece)[0];
                }else if(piece->get_side(piece) == SECOND){
                    this->history[this->turn_count][5 * i + j] = piece->get_eng_name(piece)[0]+32;
                }
            }else{
                this->history[this->turn_count][5 * i + j] = '.';
            }
        }
    }
    for (int i = 0; i < 10; i++){
        Piece piece0 = board->captured_pieces[FIRST][i];
        Piece piece1 = board->captured_pieces[SECOND][i];

        if(piece0 !=NULL){
            this->history[this->turn_count][25 + i] = piece0->get_eng_name(piece0)[0];
        }else{
            this->history[this->turn_count][25 + i] = '.';
        }

        if(piece1 !=NULL){
            this->history[this->turn_count][35 + i] = piece1->get_eng_name(piece1)[0];
        }else{
            this->history[this->turn_count][35 + i] = '.';
        }
        
    }

    for (int i = 0; i < 10; i++){
        for (int j = i+1; j < 10; j++){
            if (this->history[this->turn_count][25+i] > this->history[this->turn_count][25+j]){
                char tmp = this->history[this->turn_count][25+i];
                this->history[this->turn_count][25+i] = this->history[this->turn_count][25+j];
                this->history[this->turn_count][25+j] = tmp;
            }

            if (this->history[this->turn_count][35+i] > this->history[this->turn_count][35+j]){
                char tmp = this->history[this->turn_count][35+i];
                this->history[this->turn_count][35+i] = this->history[this->turn_count][35+j];
                this->history[this->turn_count][35+j] = tmp;
            }
        }
    }
}



//千日手を判定する
//現在の盤面と持ち駒が４度目以上ならTrue、そうでないならFalseを返す
//注意としてそのターンのrecord_boardした後に使う
static bool check_repetition(Referee this, Board board){
    int repetition_count = 1;

    for (int i=1; i <= this->turn_count; i++){
        if (strcmp(this->history[i], this->history[this->turn_count]) == 0){
            repetition_count += 1;
        }
    }

    if (repetition_count >= 4){
        return true;
    }else{
        return false;
    }
    
}


//side側が王手をかけられているか判定する
static bool judge_check(Referee this, Board board, int side){
    //sideの王の位置を探す
    bool flag1 = false;
    Point kingLoc;
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            Piece piece1 = board->board[i][j];
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
            Piece piece2 = board->board[i][j];
            if(piece2 != NULL){
                if(piece2->get_side(piece2) == 1 - side){
                    for(int k = 0; k < piece2->m->ability.length; k++){
                        Point dest;
                        add_vec_to_point(piece2->m->cur_loc,piece2->m->ability.directions[k],&dest);
                        if(is_same_point(dest,kingLoc)){
                            if(board->can_move(board,piece2,dest)){
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

    return finalFlag;
}


//歩が2個しかないことを仮定している二歩判定
bool check_double_pawn(Referee this, Board board, Piece piece, Point dest){

    //そもそも歩じゃなかったら二歩にはならない
    if(piece->get_kind(piece) != PAWN) return false;


    Piece pawn = NULL;
    
    for(int i = 0;i< 5; i++){
        for(int j = 0; j < 5; j++){
            Piece target = board->board[i][j];
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

List legal_actions(Referee this, Board board, int side){
    
    List list = new_list();
    Drop empty_drop = {0,0,NONE};
    Move empty_move = {-1,-1,-1,-1,-1};

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            Piece piece = board->board[i][j];
            if(piece == NULL) continue;

            if(piece->get_side(piece) != side) continue;

            for (int i = 0; i < piece->m->ability.length; i++){
                Point dest;
                add_vec_to_point(piece->m->cur_loc, piece->m->ability.directions[i], &dest);

                
                if (this->is_legal_move(this, board, piece, dest)){
                    if(this->can_promote(this,piece,dest)){
                        Move move = {piece->get_location(piece),dest, true};
                        add(&list,move,empty_drop);
                    }
                    Move move = {piece->get_location(piece),dest, false};
                    add(&list,move,empty_drop);
                    
                }
                       
            }
            
        }
    }


    for(int k = 0; k < 10; k++){
        Piece piece = board->captured_pieces[side][k];
        if(piece == NULL) continue;

        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                Point dest = {j, i};
                if(board->can_drop(board,piece, dest)){
                    Drop drop = {dest, piece->get_kind(piece)};
                    add(&list,empty_move,drop);
                }
            }
        }
    }

    return list;
}

bool is_checkmated(Referee this, Board board, int side){
    List list = legal_actions(this, board, side);
    if(is_empty(list)){
        free_list(list);
        return true;
    }else{
        free_list(list);
        return false;
    }
}

Referee new_referee(int turn){

    Referee instance = calloc(1, sizeof(*instance));
    instance->turn = turn;
    instance->turn_count = 1;

    instance->display_history = display_history;

    instance->update_turn = update_turn;
    instance->increment_turn_count = increment_turn_count;
    instance->get_turn = get_turn;
    instance->get_turn_count = get_turn_count;

    instance->is_legal_move = is_legal_move;
    instance->is_legal_drop = is_legal_drop;
    instance->can_promote = can_promote;
    instance->record_board = record_board;
    instance->check_repetition = check_repetition;
    instance->judge_check = judge_check;
    instance->check_double_pawn = check_double_pawn;
    instance->will_be_checked = will_be_checked;
    instance->legal_actions = legal_actions;
    instance->is_checkmated = is_checkmated;

  
    //historyを初期化
    for(int i = 0; i < 151; i++){
        for(int j = 0; j < 46; j++){
            instance->history[i][j] = '\0';
        }
    }

    return instance;
}