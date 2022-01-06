#ifndef LIST_DOUBLE_H
#define LIST_DOUBLE_H


#include <stdbool.h>

struct list_w_t;
typedef struct list_w_t *ListW;

struct list_w_t
{
    double value;
    ListW next;
};

ListW new_list_w();
void add_w(ListW, double);
int length_w(ListW);
void free_list_w(ListW);
bool is_empty_w(ListW);
double get_nth_w(ListW, int);
void save_list_w(char *file_name, ListW list);

#endif