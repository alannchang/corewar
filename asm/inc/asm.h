#ifndef _ASM_H
#define _ASM_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h> // replace with original implementations

#include "../inc/op.h"

#define TAB_CHAR 9
#define DOUBLE_QUOTE 34

#define EXECUTABLE_EXT ".cor"
#define DIRECTIVE_CHAR '.'

#define UNREQ_DIR "Unrecognized directive, please use .name or .comment only"

typedef struct source {
    const char *file_name;
    int line_num;
    FILE *fp;
    size_t buf;
    char *line;
} src;

typedef struct executable {
    char *file_name;
    FILE *fp;
} exec;

typedef struct token{
    int id;
    char *str;
    size_t len;
    int num_args;
    struct token *next;
} token;

typedef struct token_list {
    token *head;
    token *tail;
    char *err_msg;
} token_list;

// not sure if this is needed
enum token_id {
    DIRECTIVE,      // .name
    LITERAL,        // "Simple"
    INSTRUCTION,    // live
    REGISTER,       // r1
    INDEX,          // 123
    LABEL,          // l2:
    DIRECT,         // %1
};

typedef struct temp_op_s{
  char *instr;
  char nbr_args;
  args_type_t type[MAX_ARGS_NUMBER];  
} temp_op_t;

const temp_op_t temp_op_tab[] = {
  {"live", 1, {T_DIR}},
  {"ld", 2, {T_DIR | T_IND, T_REG}},
  {"st", 2, {T_REG, T_IND | T_REG}},
  {"add", 3, {T_REG, T_REG, T_REG}},
  {"sub", 3, {T_REG, T_REG, T_REG}},
  {"and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}},
  {"or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
  {"xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
  {"zjmp", 1, {T_DIR}},
  {"ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
  {"sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}},
  {"fork", 1, {T_DIR}},
  {"lld", 2, {T_DIR | T_IND, T_REG}},
  {"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
  {"lfork", 1, {T_DIR}},
  {"aff", 1, {T_REG}},
  {0, 0, {0}}
};

#endif