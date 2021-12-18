#ifndef INCLUDED_PLAYER
#define INCLUDED_PLAYER


#include <stdbool.h>

#define MAX_CAPTURED 10


struct piece_t;
typedef struct piece_t *Piece;

struct board_t;
typedef struct board_t *Board;

struct move_t;
typedef struct move_t Move;

struct drop_t;
typedef struct drop_t Drop;

struct player_t;
typedef struct player_t *Player;

typedef enum piece_kind PieceKind;

struct referee_t;
typedef struct referee_t *Referee;


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
Piece get_captured(Player this, PieceKind kind);
void remove_captured(Player this, PieceKind kind);

#endif