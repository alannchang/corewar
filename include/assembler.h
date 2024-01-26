#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

typedef struct scanner {
    const char *file_name;
    int line_ct;
    FILE *fp;
    size_t buf;
    char *line;
} scan;