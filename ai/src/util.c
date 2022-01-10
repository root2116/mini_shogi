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

char *dirname(char *file_path)
{
    char *head = file_path;
    char *p = file_path;

    int len = 0;

    while (*p != '\0')
    {
        p++;
        len++;
    }
    char *dirname = malloc(len * sizeof(char));
    char *p_dir = dirname;
    while (*p != '/')
    {
        p--;
    }

    while (head != p)
    {
        *p_dir = *head;
        head++;
        p_dir++;
    }

    return dirname;
}
