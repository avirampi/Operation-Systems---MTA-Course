# Makefile for ex3-q1 winter 2019A
CFLAGS = -Wall
CC = gcc -std=c99

OS := $(shell uname)
ifneq (,$(findstring CYGWIN_NT,$(OS)))
EXT = .exe
else
EXT =
endif

PROG = ex3_q2$(EXT)

all: $(PROG) test
	
$(PROG) : ex3_q2.o ex3_q2_functions.o ex3_q2_given.c
	@echo -n compiling $@ ... 
	@$(CC) $(CFLAGS) ex3_q2.o ex3_q2_functions.o ex3_q2_given.c -o $(PROG) -lm
	@echo  [OK]
	
clean:
	@echo removing files ...
	@rm -vf *.o ${PROG} *.log *.tmp

test:
	@echo -n going to run test ...
	@./$(PROG) > out.log 2> err.log
	@echo test finish [OK]