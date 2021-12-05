#ifndef INCLUDED_GAME
#define INCLUDED_GAME


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
    PAWN
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
} Move;

typedef struct {
    Point loc;
    PieceKind kind;
} Drop;

void start_game();

#endif