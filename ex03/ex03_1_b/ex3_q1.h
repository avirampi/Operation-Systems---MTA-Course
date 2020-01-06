#include <string.h>
#include <stdio.h>
#include <stdlib.h>         // for exit()
#include <sys/types.h>
#include <unistd.h>         // for execvp(), pipe()
#include <sys/wait.h>       // for wait()
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>        
#include <math.h>                                              

typedef char* String; // better readabilty

#define STRING_SIZE_INIT        64
#define PRINT_STRING_BASIC      "%s\n"
#define OPEN_FLAGS				O_CREAT | O_RDWR
#define SUBST_PROGRAM           "./subst"
#define ERROR_CANNOT_OPEN_PIPE  "cannot open pipe\n"

#define PIPE_WRITE_END          1
#define PIPE_READ_END           0  

void program(String argv[]);
void do_child(int* pip_child, int *pip_prev, char first, char second);
void do_last_child(int* pip_child, char first, char second, String file_name);
void do_first_child(int* pip, char first, char second);
void closePipeAndExecve(int *pip, char first, char second);