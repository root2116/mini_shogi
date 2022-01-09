#include "list.h"
#include "Game.h"

#define LEAK_DETECT
#ifdef LEAK_DETECT
#include "leakdetect.h"
#define init leak_detect_init
#define malloc(s) leak_detelc_malloc(s, __FILE__, __LINE__)
#define free leak_detect_free
#define check leak_detect_check
#else
#define init()
#define check()
#endif


#include <stdio.h>
#include <stdlib.h>

void free_list(List list){
    while(list != NULL){
        List old_list = list;
        list = list->next;
        free(old_list);
    }
}

bool is_empty(List list){
    if(list == NULL) return true;
    else return false;
}

int length(List list){
    int count = 0;
    while(list != NULL){
        count++;
        list = list->next;
    }

    return count;
}

Action* get_nth(List list, int n){
    int k = 0;
    while(list != NULL){
        if(k == n){
            return &list->action;
        }
        list = list->next;
        k++;
    }

    return NULL;
} 

void add(List *list, Move move, Drop drop){
    if(*list == NULL){
        List new_list = malloc(sizeof(*new_list));
        new_list->action.move = move;
        new_list->action.drop = drop;
        new_list->next = NULL;
        *list = new_list;
    }else{
        List new_list = malloc(sizeof(*new_list));
        new_list->action.move = move;
        new_list->action.drop = drop;
        new_list->next = *list;
        *list = new_list;
    }
    
}

List new_list(){
    
   
    return NULL;
}

