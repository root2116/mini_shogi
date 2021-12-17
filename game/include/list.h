#ifndef INCLUDED_LIST
#define INCLUDED_LIST

#include "game.h"
struct list_t;
typedef struct list_t *List;

struct list_t {
    Action action;
    List next;
};

List new_list();
void add(List*, Move, Drop);
int length(List);
void free_list(List);
bool is_empty(List);

#endif