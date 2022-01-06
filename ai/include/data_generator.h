#ifndef INCLUDED_DATA_GENERATOR
#define INCLUDED_DATA_GENERATOR
#include <stdio.h>
#include "Game.h"
#include "tensor.h"
#include "Board.h"

void genenrate_data(char* file_name1,char* file_name2 ,int num);
void save_data(FILE *fp, Game game);
void save_board_data(FILE *fp, Board board, PieceKind kind, int side);
void save_captured_data(FILE *fp, Board board, PieceKind kind, int side);
Board restore_board_from_str(char board_str[]);
Tensor load_x_train_data(char *file_name);
Vector load_x_label_data(char *file_name);
Tensor load_t_test_data(char *file_name);
Vector load_t_label_data(char *file_name);
void print_data(Tensor x);
Tensor convert_board_into_tensor(Board board);

#endif