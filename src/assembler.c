#include "../include/assembler.h"

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
General idea:

Loop until EOF:
-Use getline to read a single line
-Process line by tokenizing and identifying tokens
 +custom tokenizer that delimits all whitespace
 +classify tokens using ID numbers
-Translate tokens to machine code (binary or hexdecimal?)
-Write to executable 

*/

////////// TOKEN/LIST MANAGEMENT //////////

void init_list(token_list *list){
    list->head = NULL;
    list->tail = NULL;
}

void add_new_token(token_list *list, char *read_str){
    token *new_token = malloc(sizeof(token));
    if (new_token == NULL){
        write(2, "Error allocating memory\n", 24);
        exit(EXIT_FAILURE);
    }

    new_token->str = strdup(read_str);

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
}

void freeList(token *head){
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

    while (*ptr != '\0' && *ptr != '\n'){ // beginning of the line
        if (*ptr != ' '){

            if (*ptr == COMMENT_CHAR){ // this ignores comments
                break;
            }

            if (*ptr == '"'){ // string literals

            }

            size_t tmp_mem = 1;
            char *tmp = (char *)malloc(tmp_mem);
            if (tmp == NULL){
                write(2, "Error allocating memory\n", 24);
                exit(EXIT_FAILURE);
            }

            tmp[0] = '\0';

            while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n'){
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

            add_new_token(list, tmp);
            if (tmp != NULL){
                free(tmp);
            }
            printf("%s\n", list->tail->str); // DEBUG PRINT

        }
        ptr++;
    }
    return 0;
}

////////// GENERAL //////////

prog init_prog(const char *prog_name){
    FILE *prog_fp = fopen(prog_name, "r");
    if (prog_fp == NULL){
        write(2, "Error opening file\n", 19);
        exit(EXIT_FAILURE);
    }

    prog prog = {
        .file_name = prog_name,
        .line_ct = 0,
        .fp = prog_fp,
        .buf = 0,
        .line = NULL,
    };
    return prog;
}

char *generate_exec_name(char *av){ // THIS FUNCTION NEEDS TO BE REFACTORED PROBABLY
    char *exec_name = strdup(av);

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
        .fd = open(exec.file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644),
    };
    if (exec.fd == -1){
        write(2, "Error writing file\n", 19);
        exit(EXIT_FAILURE);
    }
    return exec;
}

void wrap_up(prog prog, exec exec, token_list *list){
    fclose(prog.fp);
    if (close(exec.fd) == -1){
        write(2, "Error closing file\n", 19);
    }
    
    if (prog.line){
        free(prog.line);
    }

    if (exec.file_name){
        free(exec.file_name);
    }

    freeList(list->head);
}

int assemble_prog(char *av){
    prog prog = init_prog(av);
    exec exec = init_exec(av);

    token_list token_list;
    init_list(&token_list);
    ssize_t read;

    while(1){
        read = getline(&prog.line, &prog.buf, prog.fp);
        if (read == -1){
            break;
        }
        prog.line_ct++;


        scan_line(&token_list, prog.line);

        write(exec.fd, prog.line, read);
    }  

    wrap_up(prog, exec, &token_list);
    return 0;        
}


int main(int ac, char **av){
    if (ac > 1){
        for (int i = 1; i < ac; i++){
            assemble_prog(av[i]);
        }
        exit(EXIT_SUCCESS);
    }
    write(2, "GIMME SOMETHING TO ASSEMBLE\n", 28);
    exit(EXIT_FAILURE);
}