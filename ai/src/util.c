#include "util.h"
#include <stdio.h>
#include <stdlib.h>


int count_lines(char *file_name)
{   
    FILE *fp = fopen(file_name, "r");
    char data[32];

    int line_count = 0;
    while (fgets(data, 32, fp) != NULL)
    {
        line_count++;
    }

    rewind(fp);

    return line_count;
}