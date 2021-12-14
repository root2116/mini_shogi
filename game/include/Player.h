#ifndef INCLUDED_PLAYER
#define INCLUDED_PLAYER

#include "Piece.h"
#include "game.h"
#include "Board.h"
#include "Referee.h"

#define MAX_CAPTURED 10

struct player_t;
typedef struct player_t *Player;

struct player_t {
    bool (*move_my_piece)(Player,Move, Board,Referee);
    bool (*drop_my_captured)(Player,Drop, Board,Referee);
    Piece *captured_pieces;
    int turn;
};

Player new_player(int turn);
static bool move_my_piece(Player , Move , Board ,Referee);
static bool drop_my_captured(Player , Drop , Board ,Referee);
Piece pop_captured(Player, PieceKind );
void add_captured(Player, Piece );

#endif