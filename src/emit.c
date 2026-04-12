#include "emit.h"

#include <stdio.h>
#include <stdbool.h>

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

void emit_mov(FILE *target_fd, char *_register, char *value_or_register) {
    fprintf(target_fd, "    mov %s, %s\n", _register, value_or_register);
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