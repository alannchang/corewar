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




int main(int ac, char **av){
    if (ac == 2){

        FILE *fp = fopen(av[1], "r");
        if (fp == NULL){
            write(2, "Error opening file\n", 19);
            exit(EXIT_FAILURE);
        }

        scan scan = {
            .file_name = av[1],
            .line_ct = 0,
            .len = 0,
            .line = NULL,
        };

        ssize_t read;

        while(1){
            read = getline(&scan.line, &scan.len, fp);
            if (read == -1){
                break;
            }
            scan.line_ct++;
            printf("%d: %s", scan.line_ct, scan.line);
        }  

        fclose(fp);
        free(scan.line);
    }
    return 0;
}