#ifndef INCLUDED_REFEREE
#define INCLUDED_REFEREE

#include "game.h"
#include "list.h"

#include <stdbool.h>

struct piece_t;
typedef struct piece_t *Piece;

struct player_t;
typedef struct player_t *Player;

struct board_t;
typedef struct board_t *Board;

struct referee_t;
typedef struct referee_t *Referee;

struct referee_t
{
    void (*display_history)(Referee);
    void (*update_turn)(Referee);
    void (*increment_turn_count)(Referee);
    int (*get_turn)(Referee);
    int (*get_turn_count)(Referee);
    bool (*is_legal_move)(Referee,Board, Piece, Point);
    bool (*is_legal_drop)(Referee, Board, Piece, Point);
    bool (*can_promote)(Referee, Piece, Point);
    void (*record_board)(Referee , Board);
    bool (*check_repetition)(Referee, Board);
    bool (*judge_check)(Referee, Board, int);
    bool (*check_double_pawn)(Referee, Board, Piece, Point);
    bool (*will_be_checked)(Referee, Board, Piece, Point);
    List (*legal_actions)(Referee,Board);
    char history[151][46];
    int turn;
    int turn_count;
};

Referee new_referee(int turn);

#endif