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

        int fd = open(av[1], O_RDONLY);
        
        int line_ct = 0;
        char *str = NULL;
        while(1){
            str = my_readline(fd);
            if (str == NULL){
                return 0;
            }
            line_ct++;
            printf("%d: %s\n", line_ct, str);
            free(str);
        }  
    }
    return 0;
}