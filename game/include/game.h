#ifndef INCLUDED_GAME
#define INCLUDED_GAME

#include <stdbool.h>

#define FIRST 0
#define SECOND 1

enum piece_kind
{
    NONE,
    KING,
    BISHOP,
    ROOK,
    GOLD,
    SILVER,
    PAWN,
    HORSE,
    DRAGON,
    PROMOTED_SILVER,
    PROMOTED_PAWN
};

typedef enum piece_kind PieceKind;


typedef struct
{
    int x;
    int y;
} Vector;

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    Point start;
    Point end;
    bool will_promote;
} Move;

typedef struct {
    Point loc;
    PieceKind kind;
} Drop;

void start_game();

#endif