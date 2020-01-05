#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    fprintf(stderr, "starting subst...\n");
    char argument_one = argv[1][0], argument_two = argv[2][0], character;
    while (scanf("%c", &character) != EOF) {
        fprintf(stderr, "got char: %c\n", character);
        if (character == argument_one) {
            printf("%c", argument_two);
        } else {
            printf("%c", character);
        }
        fflush(stdout);
    }
    fprintf(stderr, "ending subst...\n");
    return 0;
}