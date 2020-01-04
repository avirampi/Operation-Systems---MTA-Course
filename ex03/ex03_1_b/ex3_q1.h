#include <string.h>
#include <stdio.h>
#include <stdlib.h>         // for exit()
#include <sys/types.h>
#include <unistd.h>         // for execvp(), pipe()
#include <sys/wait.h>       // for wait()

typedef char* String; // better readabilty

#define STRING_SIZE_INIT        64
#define PRINT_STRING_BASIC      "%s\n"

#define ERROR_CANNOT_OPEN_PIPE  "cannot open pipe\n"

void program(String argv[]);
void create_pipes(String string, String argv[]);

inline int max (int a, int b) { return a > b ? a : b; }
inline int min (int a, int b) { return -max(-a,-b); }