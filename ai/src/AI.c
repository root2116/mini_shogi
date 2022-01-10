#include "AI.h"

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

#include <stdlib.h>
#include <stdio.h>


AI new_ai(char* name, void (*next_action)()){
    AI instance = malloc(sizeof(*instance));

    instance->name = name;
    instance->next_action = next_action;

    return instance;
}