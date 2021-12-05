#ifndef INCLUDED_PLAYER
#define INCLUDED_PLAYER

#include "Piece.h"
#include "game.h"
#include "Board.h"

#define MAX_CAPTURED 5

struct player_t;
typedef struct player_t *Player;

struct player_t {
    bool (*move_my_piece)(Player,Move, Board);
    bool (*drop_my_captured)(Player,Drop, Board);
    Piece captured_pieces[5];
    int turn;
};

Player new_player(int turn);
static bool move_my_piece(Player , Move , Board );
static bool drop_my_captured(Player , Drop , Board );
Piece pop_captured(Player, PieceKind );
void add_captured(Player, Piece );

#endif