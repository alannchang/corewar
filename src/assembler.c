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
-Translate tokens to machine code (binary or hexdecimal?)
-Write to executable 

*/

int main(int ac, char **av){
    if (ac == 2){

        FILE *prog_fp = fopen(av[1], "r");
        if (prog_fp == NULL){
            write(2, "Error opening file\n", 19);
            exit(EXIT_FAILURE);
        }

        scan scan = {
            .file_name = av[1],
            .line_ct = 0,
            .fp = prog_fp,
            .buf = 0,
            .line = NULL,
        };

        int exec_fd = open("exec_file", O_CREAT | O_WRONLY | O_TRUNC);
        if (exec_fd == -1){
            write(2, "Error writing file\n", 19);
            exit(EXIT_FAILURE);
        }

        ssize_t read;

        while(1){
            read = getline(&scan.line, &scan.buf, scan.fp);
            if (read == -1){
                break;
            }
            scan.line_ct++;

            
            printf("%d/%ld: %s", scan.line_ct, read, scan.line);
            write(exec_fd, scan.line, read);
        }  

        fclose(scan.fp);
        if (close(exec_fd) == -1){
            write(2, "Error closing file\n", 19);
        }
        free(scan.line);
    }
    exit(EXIT_SUCCESS);
}