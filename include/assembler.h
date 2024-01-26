#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include <unistd.h>

typedef struct scanner {
    char *file_name;
    int line_ct;
    size_t len;
    char *line;
} scan;