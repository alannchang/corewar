#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

#include "../include/my_readline.h"

// "Number of characters read will be set by the global variable READLINE_READ_SIZE"
int READLINE_READ_SIZE = 1;

// global variable to store stuff that comes after '\n' encountered
char* leftovers = NULL;

// used to "init (or reinitialize) global variable"
void init_my_readline() {
    if (leftovers != NULL) {
        free(leftovers);
        leftovers = NULL;
    }
}

// add everything after '\n' from read buffer to 'leftovers'
void store_leftovers(char* buffer, size_t read_result) {
    size_t leftover_length = strlen(buffer) - read_result - 1;
    leftovers = malloc((leftover_length + 1) * sizeof(char));
    memcpy(leftovers, buffer + read_result + 1, strlen(buffer) - read_result);
    leftovers[leftover_length] = '\0';
}

char* my_readline(int fd) {

    if (fd == -1) { // handle invalid file descriptor
        perror("File descriptor error");
        return NULL;
    }
    
    char* rd_line_buffer = NULL;  // stores 'line' to be returned by my_readline
    size_t total_bytes = 0; // keep track of total

    if (leftovers != NULL) { // if we got leftovers, check for '\n'

        int leftover_bytes = -1;

        for (int i = 0; i < (int)strlen(leftovers); i++) {
            if (leftovers[i] == '\n') {
                leftover_bytes = i;
                break;
            }
        }

        if (leftover_bytes > -1) { // if '\n' found

        // store everything before '\n' in leftover temp
            char* leftover_temp = malloc(((leftover_bytes + 1) * sizeof(char)));
            memcpy(leftover_temp, leftovers, leftover_bytes + 1);
            leftover_temp[leftover_bytes] = '\0';

        // store everything after '\n' in leftovers
            size_t new_leftovers_size = strlen(leftovers + leftover_bytes + 1) + 1;
            char* new_leftovers = malloc(new_leftovers_size * sizeof(char));
            memcpy(new_leftovers, leftovers + leftover_bytes + 1, new_leftovers_size);
            free(leftovers);
            leftovers = new_leftovers;
            return leftover_temp; // return everything before '\n'

        } else { // if no '\n' found
        // use contents of leftovers as base for rd_line_buffer
            rd_line_buffer = leftovers; 
            total_bytes = strlen(leftovers);
            leftovers = NULL;
        }
    }
    
    bool end_line = false; // 'true' when '\n' found

    char* rd_buffer = malloc((READLINE_READ_SIZE + 1) * sizeof(char)); // buffer used for read()
    if (rd_buffer == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    ssize_t read_result; // number of bytes read
    while ((read_result = read(fd, rd_buffer, READLINE_READ_SIZE)) > -1) { // read in READLINE_READ_SIZE blocks
        
        if (read_result == 0 && total_bytes == 0) { // End of file reached
            free(rd_buffer);
            free(rd_line_buffer);
            return NULL;
        
        } else if (read_result == 0) break;  // End of file reached, break to return rd_line_buffer
        
        rd_buffer[read_result] = '\0';

        for (int i = 0; i < read_result; i++) { // check for '\n' in read buffer
            if (rd_buffer[i] == '\n') {
                read_result = i;
                end_line = true;
                break;
            }
        }
        
        total_bytes += read_result; 

        char* temp = malloc((total_bytes + 1) * sizeof(char)); // initialize temp to store the new combined data
        if (temp == NULL) {
            perror("Memory allocation failed");
            free(rd_line_buffer);
            free(rd_buffer);
            return NULL;
        }

        if (rd_line_buffer != NULL) { // Copy the existing data from rd_line_buffer to temp
            memcpy(temp, rd_line_buffer, total_bytes - read_result);
            free(rd_line_buffer);  // Free the old storage memory
        }

        memcpy(temp + total_bytes - read_result, rd_buffer, read_result); // Copy the new data from buffer to temp
        
        rd_line_buffer = temp; // Update storage to point to the new combined data in temp
        
        if (end_line == true) { // if '\n' was found, store the rest of the buffer in leftovers
            store_leftovers(rd_buffer, read_result);   
            break;
        }
    }

    if (read_result == -1) { // handle read error
        perror("Read error");
        free(rd_buffer);
        free(rd_line_buffer);
        return NULL;
    }

    rd_line_buffer[total_bytes] = '\0'; // null terminate string to be returned
    free(rd_buffer); // free rd_buffer as it is no longer needed
    return rd_line_buffer;
}


// int main()
// {
//   char *str = NULL;

//   int fd = open("test.txt", O_RDONLY);
//   while ((str = my_readline(fd)) != NULL)
//   {
//       printf("%s\n", str);
//       free(str);
//   }
//   return 0;
// }