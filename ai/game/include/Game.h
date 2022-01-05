#ifndef INCLUDED_GAME
#define INCLUDED_GAME


struct referee_t;
typedef struct referee_t *Referee;

struct player_t;
typedef struct player_t *Player;

struct piece_t;
typedef struct piece_t *Piece;

struct board_t;
typedef struct board_t *Board;

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

struct vector2d_t;
typedef struct vector2d_t Vector2D;


struct vector2d_t
{
    int x;
    int y;
};

struct point_t;
typedef struct point_t Point;

struct point_t{
    int x;
    int y;
};

struct move_t;
typedef struct move_t Move;

struct move_t {
    Point start;
    Point end;
    bool will_promote;
};

struct drop_t;
typedef struct drop_t Drop;

struct drop_t {
    Point loc;
    PieceKind kind;
};

struct action_t;
typedef struct action_t Action;

struct action_t {
    Move move;
    Drop drop;
};

struct game_t;
typedef struct game_t *Game;

struct game_t {
    int user_num;
    Board board;
    Referee ref;
    Player players[2];
    void (*free_game)(Game);
    void (*user_vs_cpu)(Game, void (*game_ai)());
    void (*user_vs_user)(Game );
    int (*cpu_vs_cpu)(Game, void (*game_ai0)(), void (*game_ai1)(), bool verbose);
    void (*next_state)(Game, Action);
    void (*copy_game)(Game, Game);
    bool (*is_done)(Game);
    bool (*is_lose)(Game);
    bool (*is_draw)(Game);
};

Game new_game(int side);

#endif