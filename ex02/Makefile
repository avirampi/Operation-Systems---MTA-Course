# Makefile for ex1-q1 winter 2019A
CFLAGS 		= -Wall
CC 			= gcc -std=c99 -g
COMPILE 	= compile $< ...

OS := $(shell uname)
ifneq (,$(findstring CYGWIN_NT,$(OS)))
EXT = .exe
else
EXT =
endif

SRC 		= ex2_q1_helper.c ex2_q1_helper_functions.c ex2_q1.c ex2_q1_functions.c 
PROG 		= ex2_q1$(EXT)
PROG_HELPER	= ex2_q1_helper$(EXT)

all: $(PROG) $(PROG_HELPER) test

ex2_q1.o: ex2_q1.c ex2_q1_functions.c ex2_q1.h miscellaneous.h
	@echo -n $(COMPILE)
	@$(CC) $(CFLAGS) ex2_q1.c ex2_q1_functions.c -o $(PROG)
	@echo [OK]
	
ex2_q1_helper.o: ex2_q1_helper.c ex2_q1_helper_functions.c ex2_q1_helper.h miscellaneous.h
	@echo -n $(COMPILE)
	@$(CC) $(CFLAGS) ex2_q1_helper.c ex2_q1_helper_functions.c -o $(PROG_HELPER)
	@echo [OK]

$(PROG)			: ex2_q1.o
$(PROG_HELPER)	: ex2_q1_helper.o

clean:
	rm -vf *.o ${PROG} *.log *.tmp ${PROG_HELPER}
	
test:
	@echo going to run test ...
	./$(PROG) < in.txt > out.txt
	@echo [OK]