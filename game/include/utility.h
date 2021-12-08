#ifndef INCLUDED_UTILITY
#define INCLUDED_UTILITY


#include "game.h"

void add_vec_to_point(Point a, Vector v, Point *b);
Vector *convert_array_into_vectors(int array[][2], int len);
bool is_same_point(Point a, Point b);
int gcd(int x, int y);
int ctoi(char c);
void parse_move(char *input, Move *move);
void parse_drop(char *input, Drop *drop);
void multiply_array_by_minus(int array[][2], int len);
void inverse_vectors(Vector *vecs, int len);
void init_string(char *str, char init);
#endif