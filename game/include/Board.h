#ifndef INCLUDED_BOARD
#define INCLUDED_BOARD

#include "game.h"


#include <stdbool.h>
#define MAX_STACK 10


struct stack_t;
typedef struct stack_t BoardStack;



struct piece_t;
typedef struct piece_t *Piece;

struct player_t;
typedef struct player_t *Player;

struct board_t;
typedef struct board_t *Board;

struct stack_t
{
    Piece boards[MAX_STACK][5][5];
    Piece captured_pieces[MAX_STACK][2][10];
    int top;
};

struct board_t {
    void (*display_board)(Board,Player,Player);
    bool (*can_move)(Board, Piece, Point);
    Piece (*move_piece)(Board, Piece, Point);
    bool (*can_drop)(Board,Piece,Point);
    void (*drop_piece)(Board, Piece,Point);
    void (*create_next_board)(Board, Piece, Point);
    void (*clone_board)(Board);
    void (*free_board)(Board);
    void (*push_board)(Board);
    void (*restore_board)(Board);
    Piece board[5][5];
    BoardStack stack;
    Piece board_copy[5][5];
    Piece captured_pieces[2][10];
};

Board new_board();


#endif