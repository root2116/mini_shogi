#ifndef INCLUDED_SILVER
#define INCLUDED_SILVER

#include "Piece.h"
#include "Board.h"

Piece new_silver(int side);

#define SILVER_MOVE_NUM 5
static bool promote(Piece);

#endif
