#ifndef INCLUDED_BOARD
#define INCLUDED_BOARD




#include <stdbool.h>
#define MAX_STACK 10


struct stack_t;
typedef struct stack_t BoardStack;

struct point_t;
typedef struct point_t Point;

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
    Piece board[5][5];
    BoardStack stack;
    Piece captured_pieces[2][10];

    void (*display_board)(Board,Player,Player);
    bool (*can_move)(Board, Piece, Point);
    Piece (*move_piece)(Board, Piece, Point);
    bool (*can_drop)(Board,Piece,Point);
    void (*drop_piece)(Board, Piece,Point);
    void (*create_next_board)(Board, Piece, Point);
    void (*free_pieces)(Board);
    void (*push_pieces)(Board);
    void (*restore_board)(Board);
    void (*copy_board)(Board, Board);
    void (*free_board)(Board);
    void (*swap_board)(Board);
    void (*read_pre_board)(Board);
    
};

Board new_board();


#endif