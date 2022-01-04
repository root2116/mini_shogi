#ifndef INCLUDED_DATA_GENERATOR
#define INCLUDED_DATA_GENERATOR
#include <stdio.h>
#include "Game.h"


void genenrate_data(char* file_name1,char* file_name2 ,int num);
void save_data(FILE *fp, Game game);
void save_board_data(FILE *fp, Board board, PieceKind kind, int side);
void save_captured_data(FILE *fp, Board board, PieceKind kind, int side);
Board restore_board_from_str(char board_str[]);
#endif