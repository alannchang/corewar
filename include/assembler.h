#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> // replace with original implementations

typedef struct scanner {
    const char *file_name;
    int line_ct;
    FILE *fp;
    size_t buf;
    char *line;
} scan;

typedef struct token{
    int id;
    char *str;
    struct token *next;
} token;

typedef struct {
    token *head;
    token *tail;
} token_list;

enum token_types{
    REGISTER,
    INSTRUCTION,
    PARAMETER,
    INDEX,
    LABEL,
    // Single chars
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    EQUAL,
};