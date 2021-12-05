#ifndef INCLUDED_PIECE
#define INCLUDED_PIECE

#include "game.h"
#include "Board.h"
#include <stdbool.h>

typedef struct {
    Vector *directions;
    int length;
}Ability;

struct piece_member
{
    char *name;
    char *eng_name;
    PieceKind kind;
    bool promoted;
    Point cur_loc;
    int side;
    Ability ability;
};

struct piece_t;
typedef struct piece_t *Piece;





struct piece_t {
    struct piece_member *m;
    Piece (*move)(Piece,Point,Board);
    bool (*drop)(Piece,Point,Board);
    void (*betray)(Piece);
    bool (*promote)(Piece);
    Point (*get_location)(Piece);
    int (*get_side)(Piece);
    int (*get_kind)(Piece);
    char *(*get_name)(Piece);
    char *(*get_eng_name)(Piece);
    
};

Piece new_piece();
Point get_location(Piece this);
int get_side(Piece this);
int get_kind(Piece this);
char *get_name(Piece this);
char *get_eng_name(Piece this);
Piece move(Piece,Point,Board);
bool drop(Piece,Point,Board);
void betray(Piece);


#endif