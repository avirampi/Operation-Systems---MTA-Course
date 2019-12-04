# Makefile for ex2-q1 winter 2019A

CFLAGS = -Wall
CC = gcc -std=c99
ECHO = echo "going to compile for target $@"

OS := $(shell uname)
ifneq (,$(findstring CYGWIN_NT,$(OS)))
EXT = .exe
else
EXT =
endif

PROG = ex2_q1$(EXT)
HELPER = ex2_q1_helper$(EXT)
all: $(HELPER) $(PROG) test


$(PROG) : ex2_q1.o ex2_q1_functions.o
	$(CC) $(CFLAGS) ex2_q1.o ex2_q1_functions.o -o $(PROG)
	
$(HELPER) : ex2_q1_helper.o ex2_q1_helper_functions.o
	$(CC) $(CFLAGS) ex2_q1_helper.o ex2_q1_helper_functions.o -o $(HELPER)
	
clean:
	rm -vf *.o ${PROG} *.log *.tmp ${HELPER}
	
test:
	$(ECHO) going to run test...
	./$(PROG) < in.txt > out.log