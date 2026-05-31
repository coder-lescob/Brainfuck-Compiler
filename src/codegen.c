#include "codegen.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>

#include "emit.h"

void compile_bf(char *input_file, char *output_file) {

    // open input and output files
    FILE *input_fd  = fopen(input_file, "r");  CHECK_OPEN( input_fd, input_file );
    FILE *output_fd = fopen(output_file, "w"); CHECK_OPEN(output_fd, output_file);
    
    // read all the program
    char *prog = fread_all(input_fd);
    
    // close the input file after reading
    fclose(input_fd);
    
    // count the number loops in the program
    int loops = strcount(prog, '[');
    int closing_loops = strcount(prog, ']');

    // verify to avoid loop oveflows
    if (loops != closing_loops) {
        printf("loop not closed\n");
        exit(-1);
    }

    // allocates the loop stack to match this depth
    // it cannot overflow because there is only as many '[' in the program than loops pushed
    int *loop_idx_stack = calloc(loops, sizeof(int));
    memset(loop_idx_stack, 0, sizeof(int) * loops);
    int loop_stack_ptr = 0;
    int loop_idx = 0;

    // emit the data section containing the tape
    emit_section(output_fd, ".data");
    emit_line(output_fd, "tape: times 30000 dw 0");
    emit_line(output_fd, "tape_len: equ tape - $$");
    emit_empty_line(output_fd);

    // then the text section contains the code
    emit_section(output_fd, ".text");
    
    // then start is the entry point
    emit_line(output_fd, "global _start");
    emit_label(output_fd, "_start");
    emit_comment(output_fd, "r8 is the tape pointer");

    for (int i = 0; prog[i] != 0; i++) {
        emit_bf_instruction(output_fd, prog, &i, loop_idx_stack, &loop_stack_ptr, &loop_idx);
    }
    emit_label(output_fd, "_exit");

    // then place the exit syscall to end the progra without it segfaulting
    emit_comment(output_fd, "use the exit syscall to exit");
    emit_mov(output_fd, "rax", "0x3c      ; syscall exit (60)");
    emit_mov(output_fd, "rdi", "0x00      ; error code 0");
    emit_instruction(output_fd, "syscall");

    // free the loop stack and the prog string
    free(loop_idx_stack);
    free(prog);

    // close the output file
    fclose(output_fd);
}

void emit_bf_instruction(FILE *target_fd, char *prog, int *idx, int *loop_idx_stack, int *loop_stack_ptr, int *loop_idx) {

    int i = *idx;
    char c = prog[i];

    // optimization variable
    int number_of = 0;

    switch (c) {

        case '-':
        case '+': {
            // count the total number of inceaments and decrements
            number_of = 0;
            for (;prog[*idx] == '+' || prog[*idx] == '-'; (*idx)++) {
                int amount = (prog[*idx] == '+')? 1 : -1;
                number_of += amount;
            }
            
            // put the index back to avoid overshooting and skip characters
            (*idx)--;

            // just emit the add for that total
            emit_add(target_fd, "byte [tape + r8]", number_of);
        } break;

        case '<':
        case '>': {
            // count the total number of inceaments and decrements
            number_of = 0;
            for (;prog[*idx] == '>' || prog[*idx] == '<'; (*idx)++) {
                int amount = (prog[*idx] == '>')? 1 : -1;
                number_of += amount;
            }
            
            // put the index back to avoid overshooting and skip characters
            (*idx)--;

            // just emit the add for that total
            emit_add(target_fd, "r8", number_of);
        } break;

        case '[': {
            emit_loop_label(target_fd, *loop_idx, true);
            emit_instruction(target_fd, "cmp byte [tape + r8], 0");
            emit_jmp(target_fd, "je", *loop_idx, false);

            // push
            loop_idx_stack[(*loop_stack_ptr)++] = *loop_idx;
            (*loop_idx)++;
        } break;

        case ']': {
            // pop
            int close_idx = loop_idx_stack[--(*loop_stack_ptr)];

            emit_jmp(target_fd, "jmp", close_idx, true);
            emit_loop_label(target_fd, close_idx, false);
        } break;

        case '.': {
            emit_empty_line(target_fd);
            emit_mov(target_fd, "rax", "0x01        ; write syscall");
            emit_mov(target_fd, "rdi", "0x01        ; stdout");
            emit_mov(target_fd, "rsi", "tape        ; buf");
            emit_instruction(target_fd, "add rsi, r8");
            emit_mov(target_fd, "rdx", "0x01        ; a single char");
            emit_instruction(target_fd, "syscall");
            emit_empty_line(target_fd);
        } break;

        case ',': {
            emit_empty_line(target_fd);
            emit_mov(target_fd, "rax", "0x00        ; read syscall");
            emit_mov(target_fd, "rdi", "0x01        ; stdin");
            emit_mov(target_fd, "rsi", "tape        ; buf");
            emit_instruction(target_fd, "add rsi, r8");
            emit_mov(target_fd, "rdx", "0x01        ; a single char");
            emit_instruction(target_fd, "syscall");
            emit_empty_line(target_fd);
        } break;

        default:
            break;
    }
}

char *fread_all(FILE *fd) {
    // get the length of the file
    fseek(fd, 0, SEEK_END);
    size_t len = ftell(fd) + 1;
    rewind(fd);

    char *file_content = calloc(len, sizeof(char));
    
    // copy the cotent of the file in file content
    fread(file_content, len, len, fd);

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