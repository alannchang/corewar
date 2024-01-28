#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> // replace with original implementations

typedef struct program {
    const char *file_name;
    int line_ct;
    FILE *fp;
    size_t buf;
    char *line;
} prog;

typedef struct token{
    int id;
    char *str;
    int len;
    struct token *next;
} token;

typedef struct token_list{
    token *head;
    token *tail;
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
    // Single chars
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    EQUAL,
};