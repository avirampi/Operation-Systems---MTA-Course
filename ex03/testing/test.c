#include <string.h>
#include <stdio.h>
#include <stdlib.h>         // for exit()
#include <sys/types.h>
#include <unistd.h>         // for execvp(), pipe()
#include <sys/wait.h>       // for wait()
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>                                                      


void main() {
    char printer[] = "from child: hello world!";
    int pip[2];

    if (pipe(pip) < 0) {
        printf("error");
    }

    if (fork() == 0) {
        close(pip[0]);
        close(STDOUT_FILENO);
        dup2(pip[1], STDOUT_FILENO);
        printf("%s",printer);
        close(pip[1]);
        exit(0);
    }

    close(pip[1]);
    char buffer[100] = "";
    int num = read(pip[0], buffer, strlen(printer));
    close(pip[0]);
    printf("from perent: %s\n", buffer);
    printf("%d\n", num);
}