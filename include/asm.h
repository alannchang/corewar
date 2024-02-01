#ifndef _ASM_H
#define _ASM_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h> // replace with original implementations

#include "../include/op.h"

#define TAB_CHAR 9
#define DOUBLE_QUOTE 34

#define EXECUTABLE_EXT ".cor"
#define DIRECTIVE_CHAR '.'

typedef struct source {
    const char *file_name;
    int line_num;
    FILE *fp;
    size_t buf;
    char *line;
} src;

typedef struct executable {
    char *file_name;
    int fd;
} exec;

typedef struct token{
    int id;
    char *str;
    size_t len;
    struct token *next;
} token;

typedef struct token_list{
    token *head;
    token *tail;
    char *err_msg;
} token_list;

// not sure if this is needed
enum token_types{
    REGISTER,
    INSTRUCTION,
    PARAMETER,
    INDEX,
    LABEL, // begin with ':'
    COMMENT, // begin with '#'
    SEPARATOR, // ','
    DIRECT, // begin with '%'
};

#endif