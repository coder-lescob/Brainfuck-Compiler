#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#define INPUT_FILE  0
#define OUTPUT_FILE 1

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <input file> -o <output file>", argv[0]);
        return 1;
    }

    // first we need to parse argv
    char *args[2];
    bool is_output = false;

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-o") == 0) {
            is_output = true;
            continue;
        }

        int j = is_output ? OUTPUT_FILE : INPUT_FILE;
        size_t num_bytes = strlen(argv[i]);

        args[j] = malloc(num_bytes + 1);
        args[num_bytes] = 0;
        memcpy(args[j], argv[i], num_bytes);
        
        is_output = false;
    }

    printf("input: %s \t output: %s\n", args[INPUT_FILE], args[OUTPUT_FILE]);

    for (int i = 0; i < 2; i++) {
        free(args[i]);
    }
}