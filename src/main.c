#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "args.h"
#include "emit.h"
#include "codegen.h"

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