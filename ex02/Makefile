# Makefile for ex2-q1 winter 2019A
CFLAGS = -Wall
CC = gcc -std=c99

OS := $(shell uname)
ifneq (,$(findstring CYGWIN_NT,$(OS)))
EXT = .exe
else
EXT =
endif

PROG = ex2_q1$(EXT)
HELPER = ex2_q1_helper$(EXT)

all: $(PROG) $(HELPER) test
	
$(PROG) : ex2_q1.o ex2_q1_functions.o ex2_q1.h miscellaneous.h
	@echo -n compiling $@ ... 
	@$(CC) $(CFLAGS) ex2_q1.o ex2_q1_functions.o -o $(PROG)
	@echo  [OK]
	
$(HELPER) : ex2_q1_helper.o ex2_q1_helper_functions.o ex2_q1_helper.h miscellaneous.h
	@echo -n compiling $@ ... 
	@$(CC) $(CFLAGS) ex2_q1_helper.o ex2_q1_helper_functions.o -o $(HELPER)
	@echo  [OK]
	
clean:
	@echo removing files ...
	@rm -vf *.o ${PROG} *.log *.tmp ${HELPER}
	
test:
	@echo -n going to run test ...
	@./$(PROG) < in.txt > out.log
	@echo test finish [OK]