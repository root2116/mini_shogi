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
    bool (*can_move)(Board, Piece, Point);
    Piece (*move_piece)(Board, Piece, Point);
    bool (*can_drop)(Board,Piece,Point);
    void (*drop_piece)(Board, Piece,Point);
    void (*copy_board)(Board);
    void (*clone_board)(Board);
    void (*restore_board)(Board this);
    void (*free_board)(Board this);
    Piece board[5][5];
    Piece board_copy[5][5];
    Piece captured_pieces[2][10];
};

Board new_board();


#endif