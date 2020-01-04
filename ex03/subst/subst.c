#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRING_SIZE_INIT        64
#define PRINT_STRING_BASIC      "%s\n"
typedef char*                   String; // better readabilty

int main(int argc, String argv[]) {
    char argument_one = argv[1][0], argument_two = argv[2][0], character;

    for (size_t initializer_size = STRING_SIZE_INIT; ; initializer_size = STRING_SIZE_INIT) {
        String string = (char*)calloc(1, initializer_size), current_position = string;

        while ((scanf("%c", &character) != EOF) && character != '\n') {
            if (strlen(string) + 1 > initializer_size) {
                string = (char*)realloc(string, initializer_size = initializer_size * 2);
            }

            string = strncat(string, &character, 1);
        }

        while (current_position = strchr(current_position, argument_one)){
            *current_position = argument_two;
        }

        printf(PRINT_STRING_BASIC, string);
        free(string);
    }

    return 0;
}