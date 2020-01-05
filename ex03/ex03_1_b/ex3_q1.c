#include "ex3_q1.h"

int main(int argc, String argv[]) {
    program(argv);
    return 0;
}

size_t min(size_t a, size_t b) { return (a > b ? b : a); }

void program(String argv[]) {
    size_t program_num = min(strlen(argv[1]), strlen(argv[2]));
    int **programs_pipes = (int**)calloc(program_num - 1, sizeof(int*));

    programs_pipes[0] = (int*)calloc(2, sizeof(int));
    if (pipe(programs_pipes[0]) == -1) {
        fprintf(stderr, ERROR_CANNOT_OPEN_PIPE);
        exit(EXIT_FAILURE);
    }

    if (program_num == 1) {
        close(programs_pipes[0][PIPE_WRITE_END]);
        programs_pipes[0][PIPE_WRITE_END] = open(argv[3], O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
    }

    if (fork() == 0) {
        do_first(programs_pipes[0], argv[1][0], argv[2][0]);
    }

    close(programs_pipes[0][PIPE_WRITE_END]);

    for (size_t program_index = 1; program_index < program_num - 1; ++program_index) {
        programs_pipes[program_index] = (int*)calloc(2, sizeof(int));
        
        if (pipe(programs_pipes[program_index]) == -1) {
            fprintf(stderr, ERROR_CANNOT_OPEN_PIPE);
            exit(EXIT_FAILURE) ;
        }

        if (fork() == 0) {
            do_child(programs_pipes[program_index], programs_pipes[program_index - 1], argv[1][program_index], argv[2][program_index]);
        }

        //parent proccess only
        close(programs_pipes[program_index][PIPE_WRITE_END]);
        close(programs_pipes[program_index - 1][PIPE_READ_END]);
    }
    
    if (program_num > 1 && fork() == 0) {
        do_last_child(programs_pipes[(program_num > 1 ? program_num - 2 : 0)], argv[1][program_num - 1], argv[2][program_num - 1], argv[3]);
    }
    close(programs_pipes[(program_num > 1 ? program_num - 2 : 0)][PIPE_READ_END]);

    for (size_t i = 0; i < program_num; ++i) {
        wait(NULL);
    }
}


void do_first(int* pip, char first, char second) {
    close(STDOUT_FILENO);

    dup2(pip[PIPE_WRITE_END], STDOUT_FILENO);

    close(pip[PIPE_READ_END]);
    close(pip[PIPE_WRITE_END]);

    String args[4] = { SUBST_PROGRAM, &first, &second, NULL };
    execve(SUBST_PROGRAM, args, NULL);
}

void do_child(int* pip_child, int *pip_prev, char first, char second) {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);

    dup2(pip_prev[PIPE_READ_END], STDIN_FILENO);
    dup2(pip_child[PIPE_WRITE_END], STDOUT_FILENO);

    close(pip_child[PIPE_READ_END]);
    close(pip_child[PIPE_WRITE_END]);

    String args[4] = { SUBST_PROGRAM, &first, &second, NULL };
    execve(SUBST_PROGRAM, args, NULL);
}

void do_last_child(int* pip_child, char first, char second, String file_name) {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);

    dup2(pip_child[PIPE_READ_END], STDIN_FILENO);
    open(file_name, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);

    close(pip_child[PIPE_READ_END]);
    close(pip_child[PIPE_WRITE_END]);

    String args[4] = { SUBST_PROGRAM, &first, &second, NULL };
    execve(SUBST_PROGRAM, args, NULL);   
}