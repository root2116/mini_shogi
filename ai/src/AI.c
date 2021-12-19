#include "AI.h"

#include <stdlib.h>
#include <stdio.h>


AI new_ai(char* name, void (*next_action)()){
    AI instance = calloc(1,sizeof(*instance));

    instance->name = name;
    instance->next_action = next_action;

    return instance;
}