#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include <stdlib.h>

// checks if a file has been correcly opened
#define CHECK_OPEN(FD, FILE_NAME)                                                                                                       \
    if (FD == NULL) {                                                                                                                   \
        printf("Unable to open file: '%s': ", FILE_NAME);                                                                               \
        fflush(stdout); /* to flush stdout is mendatory since it refreshes the buffer and displays our text before the errno message */ \
        perror("");                                                                                                                     \
        exit(-1);                                                                                                                       \
    }

/**
 * compiles a bf code stored in a file called `input_file` in an assembly file called `output_file`
 */
void compile_bf(char *input_file, char *output_file);

/**
 * emits the current bf instruction to `target_fd`
 */
void emit_bf_instruction(FILE *target_fd, char *prog, int *idx, int *loop_idx_stack, int *loop_stack_ptr, int *loop_idx);

/**
 * read all the characters from a stream `fd` in a string
 * @note the string must be freed using `free()`
 */
char *fread_all(FILE *fd);

/**
 * count the number of occurances of `c` in `str`
 */
int strcount(char *str, char c);

#endif