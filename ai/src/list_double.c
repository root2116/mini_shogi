#include "list_double.h"

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
#include <math.h>


void free_list_w(ListW list)
{
    while (list != NULL)
    {
        ListW old_list = list;
        list = list->next;
        free(old_list);
    }
}

bool is_empty_w(ListW list)
{
    if (isnan(list->value))
        return true;
    else
        return false;
}

int length_w(ListW list)
{
    int count = 0;
    while (list != NULL)
    {
        count++;
        list = list->next;
    }

    return count;
}

double get_nth_w(ListW list, int n)
{
    int k = 0;
    while (list != NULL)
    {
        if (k == n)
        {
            return list->value;
        }
        list = list->next;
        k++;
    }

    return NAN;
}

void add_w(ListW list, double value)
{

    if(isnan(value)){
        fprintf(stderr, "Invalid value.(add)\n");
        return;
    }

    while(list->next != NULL){
        list = list->next;
    }

    if(isnan(list->value)){
        list->value = value;
    }else{
        ListW new_list = malloc(sizeof(*new_list));
        new_list->value = value;
        new_list->next = NULL;
        list->next = new_list;
    }

}

void save_list_w(char* file_name,ListW list){
    FILE *fp = fopen(file_name, "w");

    while (list != NULL)
    {
        fprintf(fp, "%lf\n", list->value);
        list = list->next;
    }

    fclose(fp);
}

ListW new_list_w()
{
    ListW new_list = malloc(sizeof(*new_list));

    new_list->value = NAN;
    new_list->next = NULL;

    return new_list;
    
}