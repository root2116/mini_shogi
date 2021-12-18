#ifndef INCLUDED_PIECE
#define INCLUDED_PIECE

#include "Game.h"

struct referee_t;
typedef struct referee_t *Referee;

struct board_t;
typedef struct board_t *Board;

struct vector_t;
typedef struct vector_t Vector;

struct point_t;
typedef struct point_t Point;

#include <stdbool.h>

#define GOLD_MOVE_NUM 6
#define ROOK_MOVE_NUM 16
#define PROMOTED_ROOK_MOVE_NUM 20
#define BISHOP_MOVE_NUM 16
#define PROMOTED_BISHOP_MOVE_NUM 20
#define SILVER_MOVE_NUM 5
#define PAWN_MOVE_NUM 1
#define KING_MOVE_NUM 8

#define PIECE_NUM 12

typedef struct {
    Vector *directions;
    int length;
}Ability;

struct piece_member
{
    char *name;
    char *eng_name;
    char *idle_eng_name;
    PieceKind kind;
    PieceKind idle_kind;
    bool promoted;
    Point cur_loc;
    int side;
    Ability ability;
    Ability idle_ability;
};

struct piece_t;
typedef struct piece_t *Piece;





struct piece_t {
    struct piece_member *m;
    bool (*move)(Piece,Point,Board,Referee,bool,Piece*);
    bool (*drop)(Piece,Point,Board,Referee);
    void (*betray)(Piece);
    void (*set_cur_loc_outside)(Piece);
    void (*promote)(Piece);
    Piece (*clone_piece)(Piece);
    void (*free_piece)(Piece);
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
bool move(Piece,Point,Board,Referee,bool,Piece*);
bool drop(Piece,Point,Board,Referee);
void betray(Piece);
void set_cur_loc_outside(Piece);
void promote(Piece);
void demote(Piece);
Piece clone_piece(Piece);
void free_piece(Piece);
void copy_ability(Ability *original, Ability *dest);void swap_piece_attributes(Piece this);

#endif