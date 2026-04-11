#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define NUM_ARGS 2

#define INPUT_FILE  0
#define OUTPUT_FILE 1

void parse_args(char **args_buf, int argc, char **argv) {
    bool is_output = false;

    for (int i = 1; i < argc; i++) {

        // search for the flag '-o'
        if (strcmp(argv[i], "-o") == 0) {
            is_output = true;
            continue;
        }

        // choose if it is an input or output file
        int j = is_output ? OUTPUT_FILE : INPUT_FILE;

        // output file has been taken (or does nothing if is_output is false)
        is_output = false;

        // somehow the argument has already been filled
        if (args_buf[j] != NULL) {
            // what the heck ?!
            // we have 2 options, overwrite or do nothing !
            continue; // I choose to do nothing because it is simpler
        }
        
        // get the length of the argument
        size_t num_bytes = strlen(argv[i]);

        // create the argument and copy from argv to the argument
        args_buf[j] = malloc(num_bytes + 1);
        memset(args_buf[j], 0, num_bytes + 1);
        memcpy(args_buf[j], argv[i], num_bytes);
    }
}

void free_args_buf(char **args_buf) {
    for (int i = 0; i < NUM_ARGS; i++) {
        free(args_buf[i]);
    }
}

void emit_section(FILE *target_fd, char *section_name) {
    fprintf(target_fd, "section %s\n", section_name);
}

void emit_comment(FILE *target_fd, char *comment) {
    fprintf(target_fd, "; %s\n", comment);
}

void emit_line(FILE *target_fd, char *code) {
    fprintf(target_fd, "%s\n", code);
}

void emit_instruction(FILE *target_fd, char *code) {
    fprintf(target_fd, "    %s\n", code);
}

void emit_jmp(FILE *target_fd, char *type_jmp, int loop_idx, bool open) {
    fprintf(target_fd, "    %s %s_%d\n", type_jmp, open ? "open" : "close", loop_idx);
}

void emit_empty_line(FILE *target_fd) {
    fprintf(target_fd, "\n");
}

void emit_label(FILE *target_fd, char *label) {
    fprintf(target_fd, "%s:\n", label);
}

void emit_loop_label(FILE *target_fd, int loop_idx, bool open) {
    fprintf(target_fd, "%s_%d:\n", open ? "open" : "close", loop_idx);
}

// checks if a file has been correcly opened
#define CHECK_OPEN(FD, FILE_NAME)                                                                                                       \
    if (FD == NULL) {                                                                                                                   \
        printf("Unable to open file: '%s': ", FILE_NAME);                                                                               \
        fflush(stdout); /* to flush stdout is mendatory since it refreshes the buffer and displays our text before the errno message */ \
        perror("");                                                                                                                     \
        exit(-1);                                                                                                                       \
    }

char *fread_all(FILE *fd) {
    size_t buf_size = 8;
    char *file_content = malloc(buf_size);
    memset(file_content, 0, buf_size);
    char c = 0;

    for (size_t i = 0; (c = fgetc(fd)) != EOF; i++) {
        printf("%s\n", file_content);

        if (i >= buf_size) {
            size_t old_buf_size = buf_size;
            buf_size <<= 1; // x2

            char *new_ptr = malloc(buf_size);
            memset(new_ptr, 0, buf_size);
            memcpy(new_ptr, file_content, old_buf_size);
            free(file_content);
            file_content = new_ptr;
        }

        file_content[i] = c;
    }

    return file_content;
}

int strcount(char *str, char c) {
    int size = 0;

    // count the number of occurances of c in str
    for (int i = 0; str[i] != 0; i++) {
        if (str[i] == c) size++;
    }

    return size;
}

void compile_bf(char *input_file, char *output_file) {
    char c;

    FILE *input_fd = fopen(input_file, "r");   CHECK_OPEN(input_fd, input_file);
    FILE *output_fd = fopen(output_file, "w"); CHECK_OPEN(output_fd, output_file);
    
    // read all the program
    char *prog = fread_all(input_fd);
    
    // count the number loop
    int loops = strcount(prog, '[');

    // allocates the loop stack
    int *loop_idx_stack = calloc(loops, sizeof(int));
    memset(loop_idx_stack, 0, sizeof(int) * loops);
    int loop_stack_ptr = 0;
    int loop_idx = 0;

    emit_section(output_fd, ".data");
    emit_line(output_fd, "tape: times 30000 dw 0");
    emit_line(output_fd, "tape_len: equ tape - $$");
    emit_empty_line(output_fd);
    emit_section(output_fd, ".text");
    
    emit_line(output_fd, "global _start");
    emit_label(output_fd, "_start");
    emit_comment(output_fd, "r8 is the tape pointer");
    for (int i = 0; (c = prog[i]) != 0; i++) {
        switch (c)
        {
            case '+': {
                emit_instruction(output_fd, "inc byte [tape + r8]");
            } break;

            case '-': {
                emit_instruction(output_fd, "dec byte [tape + r8]");
            } break;

            case '>': {
                emit_instruction(output_fd, "inc r8");
            } break;

            case '<': {
                emit_instruction(output_fd, "dec r8");
            } break;

            case '[': {
                emit_loop_label(output_fd, loop_idx, true);
                emit_instruction(output_fd, "cmp byte [tape + r8], 0");
                emit_jmp(output_fd, "je", loop_idx, false);

                // push
                loop_idx_stack[loop_stack_ptr++] = loop_idx;
                loop_idx++;
            } break;

            case ']': {
                // pop
                int close_idx = loop_idx_stack[--loop_stack_ptr];

                emit_jmp(output_fd, "jmp", close_idx, true);
                emit_loop_label(output_fd, close_idx, false);
            } break;

            default:
                break;
        }
    }
    emit_label(output_fd, "_exit");

    emit_comment(output_fd, "use the exit sycall to exit");
    emit_instruction(output_fd, "mov rax, 0x3c \t \t ; syscall exit (60)");
    emit_instruction(output_fd, "mov rdi, 0x00 \t \t ; error code 0");
    emit_instruction(output_fd, "syscall");

    free(loop_idx_stack);
    free(prog);

    fclose(input_fd);
    fclose(output_fd);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("usage: %s <input file> -o <output file>\n", argv[0]);
        return 1;
    }

    // first we need to parse argv
    char *args[NUM_ARGS] = { NULL, NULL };
    parse_args(args, argc, argv);

    compile_bf(args[INPUT_FILE], args[OUTPUT_FILE]);

    free_args_buf(args);
}