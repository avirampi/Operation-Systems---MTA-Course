#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    char argument_one = argv[1][0], argument_two = argv[2][0], character;
    while (scanf("%c", &character) != EOF) {
        if (character == argument_one) {
            printf("%c", argument_two);
        } else {
            printf("%c", character);
        }
        fflush(stdout);
    }
    return 0;
}