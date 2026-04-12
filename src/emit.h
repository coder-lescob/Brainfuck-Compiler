#ifndef EMIT_H
#define EMIT_H

#include <stdio.h>
#include <stdbool.h>

/**
 * emits the section header for section `section_name`
 */
void emit_section(FILE *target_fd, char *section_name);

/**
 * emits a comment of the form `; comment`
 */
void emit_comment(FILE *target_fd, char *comment);

/**
 * emits a line `code`
 */
void emit_line(FILE *target_fd, char *code);

/**
 * emits any kind of instruction
 */
void emit_instruction(FILE *target_fd, char *code);

/**
 * emits a mov instruction
 */
void emit_mov(FILE *target_fd, char *_register, char *value_or_register);

/**
 * emits an add instruction
 */
void emit_add(FILE *target_fd, char *address_or_reg, int amount);

/**
 * emit a jump instruction to a loop
 * 
 *  `{type_jmp} {open | close}_{loop_idx}` 
 */
void emit_jmp(FILE *target_fd, char *type_jmp, int loop_idx, bool open);

/**
 * emits an empty line
 */
void emit_empty_line(FILE *target_fd);

/**
 * emits a label of the forme `label_name:`
 */
void emit_label(FILE *target_fd, char *label);

/**
 * emits the label of a loop in the form `{open | close}_{loop_idx}:`
 */
void emit_loop_label(FILE *target_fd, int loop_idx, bool open);

#endif