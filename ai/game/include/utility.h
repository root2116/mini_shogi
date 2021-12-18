#ifndef INCLUDED_UTILITY
#define INCLUDED_UTILITY



struct piece_t;
typedef struct piece_t *Piece;

struct point_t;
typedef struct point_t Point;

struct vector_t;
typedef struct vector_t Vector;

struct move_t;
typedef struct move_t Move;

struct drop_t;
typedef struct drop_t Drop;

struct action_t;
typedef struct action_t Action;

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
void *convert_action_into_string(Action,char*);
int argmax(int*, int);
#endif