#include "optimizer.h"

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



Optimizer new_optimizer(double lr){
    Optimizer instance = malloc(sizeof(*instance));

    instance->lr = lr;
    instance->iter = 0;

    instance->beta1 = 0;
    instance->beta2 = 0;
    instance->m = NULL;
    instance->v = NULL;

    return instance;
}