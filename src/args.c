#include "args.h"

#include <malloc.h>
#include <string.h>
#include <stdbool.h>

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