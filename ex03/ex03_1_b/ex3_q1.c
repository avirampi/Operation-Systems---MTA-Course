#include "ex3_q1.h"

int main(int argc, String argv[]) {
    program(argv);
    return 0;
}

void program(String argv[]) {

    for (size_t initializer_size = STRING_SIZE_INIT; ; initializer_size = STRING_SIZE_INIT) {
        String string = (char*)calloc(1, initializer_size), current_position = string;
        char character;

        while ((scanf("%c", &character) != EOF) && character != '\n') {
            if (strlen(string) + 1 > initializer_size) {
                string = (char*)realloc(string, initializer_size = initializer_size * 2);
            }

            string = strncat(string, &character, 1);
        }

        create_pipes(string, argv);

        printf(PRINT_STRING_BASIC, string);
        free(string);
    }
}

void create_pipes(String string, String argv[]) {
    size_t program_num = min(strlen(argv[1]), strlen(argv[2]));
    int **programs_pipes = (int**)calloc(program_num, sizeof(int*));

    for (size_t program_index = 0; program_index < program_num; ++program_index) {
        programs_pipes[program_index] = (int*)calloc(2, sizeof(int));
        
        if (pipe(programs_pipes[program_index]) == -1) {
            fprintf(stderr, ERROR_CANNOT_OPEN_PIPE) ;
            exit(EXIT_FAILURE) ;
        }
    }
}