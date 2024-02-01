#include "../include/asm.h"
#include "../include/op.h"

/*
Authorized Functions
● (f)open
● read
● write
● getline
● lseek
● fseek
● (f)close
● malloc
● realloc
● free
● exit
*/

/*
Code Outline:

Loop until EOF:
-Use getline to read a single line
-Process line
 +tokenize, delimiting all whitespaces and newline chars
 +classify tokens using ID numbers
 +validate tokens
 +break loop if invalid tokens found, write error to stderr 

Write to executable:
-Translate tokens to machine code (binary or hexdecimal)

*/

////////// TOKEN/LIST MANAGEMENT //////////

void init_list(token_list *list){
    list->head = NULL;
    list->tail = NULL;
    list->err_msg = NULL;
}

int validate_label(const char *str){

    while (*str != LABEL_CHAR){
        bool found = false;

        for (const char *allowed = LABEL_CHARS; *allowed != LABEL_CHAR; allowed++) {
            if (*str == *allowed){
                found = true;
                break;
            }
        }

        if (!found) {
            return -1;
        }
        str++;
    }
    return 0;
}

int validate_reg(const char *str){
    str++;
    while(*str != '\0'){
        if (!isdigit(*str)){
            return -1; // should add a way to check if valid register number
        }
        str++;
    }
    return 0;
}

int id_validate(token_list *list){
    char *str = list->tail->str;
    size_t len = list->tail->len;

    switch(str[0]){
        case DIRECTIVE_CHAR: 
            list->tail->id = DIRECTIVE;
            return 0;
        case DIRECT_CHAR: 
            list->tail->id = DIRECT;
            return 0;
        case 'r':
            if (isdigit(str[1])){
                list->tail->id = REGISTER;
                if (validate_reg(str) != 0){
                    return -1;
                }
                return 0;
            }
    }

    if (str[len - 1] == LABEL_CHAR){
        list->tail->id = LABEL;
        if (validate_label(str) != 0){
            return -1;
        }
        return 0;
    }

    for (int i = 0; temp_op_tab[i].instr != 0; i++){
        if (strcmp(temp_op_tab[i].instr, str) == 0){
            list->tail->id = INSTRUCTION;
            list->tail->num_args = temp_op_tab[i].nbr_args;
        }
    }
    

    return 0;
}

void add_token(token_list *list, char *read_str){
    token *new_token = malloc(sizeof(token));
    if (new_token == NULL){
        write(2, "Error allocating memory\n", 24);
        exit(EXIT_FAILURE);
    }

    new_token->id = -1;
    new_token->str = strdup(read_str);  // use strndup instead??
    new_token->len = strlen(read_str);
    new_token->num_args = 0;

    if (new_token->str == NULL){
        write(2, "Error allocating memory\n", 24);
        exit(EXIT_FAILURE);
    }

    new_token->next = NULL;
    if (list->head == NULL){ // empty list
        list->head = new_token;
        list->tail = new_token;
    } else {
        list->tail->next = new_token;
        list->tail = new_token;
    }

    id_validate(list);

}

void free_list(token *head){
    while (head != NULL){
        token *temp = head;
        head = head->next;
        free(temp->str);
        free(temp);
    }
}

////////// SCANNER //////////

int scan_line(token_list *list, char *read_str){
    char *ptr = read_str;
    bool str_found = false;

    while (*ptr != '\0' && *ptr != '\n'){ // beginning of the line
        if (*ptr != ' ' && *ptr != TAB_CHAR){

            if (*ptr == COMMENT_CHAR){ // ignore comment char to newline char
                break;
            }

            size_t tmp_mem = 1;
            char *tmp = (char *)malloc(tmp_mem);
            if (tmp == NULL){
                write(2, "Error allocating memory\n", 24);
                exit(EXIT_FAILURE);
            }

            tmp[0] = '\0';

            if (*ptr == DOUBLE_QUOTE){
                str_found = true;
                ptr++;
                while (*ptr != DOUBLE_QUOTE){ // will run into problems with multi line strings
                    tmp_mem++;
                    tmp = (char *)realloc(tmp, tmp_mem);
                    if (tmp == NULL){
                        write(2, "Error allocating memory\n", 24);
                        exit(EXIT_FAILURE);
                    }

                    tmp[tmp_mem - 2] = *ptr;
                    tmp[tmp_mem - 1] = '\0'; 
            
                    ptr++;
                }
            } else {

                while (*ptr != ' ' && *ptr != TAB_CHAR && *ptr != SEPARATOR_CHAR && *ptr != '\0' && *ptr != '\n'){ // tokenize
                    tmp_mem++;
                    tmp = (char *)realloc(tmp, tmp_mem);
                    if (tmp == NULL){
                        write(2, "Error allocating memory\n", 24);
                        exit(EXIT_FAILURE);
                    }

                    tmp[tmp_mem - 2] = *ptr;
                    tmp[tmp_mem - 1] = '\0'; 
            
                    ptr++;
                }
            }

            add_token(list, tmp);
            if (tmp != NULL){
                free(tmp);
            }
            if (str_found){
                list->tail->id = LITERAL;
            }
            printf("%s id = %d args = %d\n", list->tail->str, list->tail->id, list->tail->num_args); // DEBUG PRINT

        }
        ptr++;
    }
    return 0;
}

////////// GENERAL //////////

src init_src(const char *src_name){
    FILE *src_fp = fopen(src_name, "r");
    if (src_fp == NULL){
        write(2, "Error opening file\n", 19);
        exit(EXIT_FAILURE);
    }

    src src = {
        .file_name = src_name,
        .line_num = 0,
        .fp = src_fp,
        .buf = 0,
        .line = NULL,
    };
    return src;
}

char *generate_exec_name(char *av){ // THIS FUNCTION NEEDS TO BE REFACTORED PROBABLY
    char *exec_name = strdup(av); // use strndup instead??

    int i = 0;
    while(exec_name[i] != '\0'){
        if (exec_name[i] == '.'){
            exec_name = (char *)realloc(exec_name, i + strlen(EXECUTABLE_EXT) + 1);
            int j = 0;
            while(EXECUTABLE_EXT[j] != '\0'){
                exec_name[i] = EXECUTABLE_EXT[j];
                i++;
                j++;
            }
            exec_name[i] = '\0';
            return exec_name;
        }
        i++;
    }
    exec_name[i] = '.';
    exec_name = (char *)realloc(exec_name, i + strlen(EXECUTABLE_EXT) + 1);
    int j = 0;
    while(EXECUTABLE_EXT[j] != '\0'){
        exec_name[i] = EXECUTABLE_EXT[j];
        i++;
        j++;
    }
    exec_name[i] = '\0';
    return exec_name;
}

exec init_exec(char *av){
    exec exec = {
        .file_name = generate_exec_name(av),
        .fd = open(exec.file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644), // consider using fopen() with "wb" to write binary?
    };
    if (exec.fd == -1){
        write(2, "Error writing file\n", 19);
        exit(EXIT_FAILURE);
    }
    return exec;
}

void wrap_up(src src, exec exec, token_list *list){
    fclose(src.fp);
    if (close(exec.fd) == -1){
        write(2, "Error closing file\n", 19);
    }
    
    if (src.line){
        free(src.line);
    }

    if (exec.file_name){
        free(exec.file_name);
    }

    free_list(list->head);
}

int scan_src(char *av){
    src src = init_src(av);

    token_list token_list;
    init_list(&token_list);
    ssize_t read;

    while(1){
        read = getline(&src.line, &src.buf, src.fp);
        if (read == -1){
            break;
        }
        src.line_num++;


        // TOKENIZE/IDENTIFY
        scan_line(&token_list, src.line);
         
    }  
    // WRITE EXECUTABLE AFTER VALIDATING SRC FILE, TOKENS, ETC.
    exec exec = init_exec(av);
    // write(exec.fd, src.line, read); // use fwrite() if using fopen()
    wrap_up(src, exec, &token_list);
    return 0;        
}

bool valid_asm_name(const char *file_name){
    size_t len = strlen(file_name);
    return (len > 2 && file_name[len - 1] == 's' && file_name[len - 2] == '.');
}


int main(int ac, char **av){
    if (ac == 2 && valid_asm_name(av[1])){
        if (scan_src(av[1]) != 0){
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    write(2, "Invalid number of arguments\n", 28);
    exit(EXIT_FAILURE);
}