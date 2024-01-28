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

/////////////////////////////////// TOKEN/LIST MANAGEMENT ///////////////////////////////////

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

/////////////////////////////////// SCANNER ///////////////////////////////////

int scan_line(token_list *list, char *read_str){
    char *ptr = read_str;

    while (*ptr != '\0' && *ptr != '\n'){
        if (*ptr != ' '){

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
            free(tmp);
            printf("%s\n", list->tail->str); // DEBUG PRINT

        }
        ptr++;
    }
    return 0;
}

int main(int ac, char **av){
    if (ac == 2){

        prog prog = init_prog(av[1]);

        int exec_fd = open("exec_file", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (exec_fd == -1){
            write(2, "Error writing file\n", 19);
            exit(EXIT_FAILURE);
        }

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

            write(exec_fd, prog.line, read);
        }  

        fclose(prog.fp);
        if (close(exec_fd) == -1){
            write(2, "Error closing file\n", 19);
        }
        free(prog.line);
        freeList(token_list.head);
    }
    exit(EXIT_SUCCESS);
}