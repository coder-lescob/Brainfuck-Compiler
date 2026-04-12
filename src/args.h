#ifndef ARGS_H
#define ARGS_H

#define NUM_ARGS 2

#define INPUT_FILE  0
#define OUTPUT_FILE 1

/**
 * parse args from argv in an array formatted like so `[INPUT_FILE, OUTPUT_FILE]`
 * @note the args_buffer must be freed with `free_args_buf()`
 */
void parse_args(char **args_buf, int argc, char **argv);

/**
 * frees the args buffer.
 */
void free_args_buf(char **args_buf);

#endif