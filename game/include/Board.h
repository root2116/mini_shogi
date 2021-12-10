#ifndef INCLUDED_BOARD
#define INCLUDED_BOARD

#include "game.h"


#include <stdbool.h>
struct piece_t;
typedef struct piece_t *Piece;
struct player_t;
typedef struct player_t *Player;

struct board_t;
typedef struct board_t *Board;

struct board_t {
    void (*display_board)(Board,Player,Player);
    void (*update_turn)(Board);
    void (*increment_turn_count)(Board);
    int (*get_turn)(Board);
    int (*get_turn_count)(Board);
    bool (*can_move)(Board, Piece, Point);
    Piece (*move_piece)(Board, Piece, Point);
    bool (*can_drop)(Board,Point);
    void (*drop_piece)(Board, Piece,Point);
    bool (*can_promote)(Board,Piece,Point);
    void (*record_board)(Board);
    bool (*check_repetition)(Board);
    bool (*judge_check)(Board,int);
    Piece board[5][5];
    char history[151][26];
    int turn;
    int turn_count;
};

Board new_board(int turn);


#endif