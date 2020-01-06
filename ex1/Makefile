# Makefile for ex1-q1 winter 2019A


CFLAGS = -Wall
CC = gcc -std=c99
ECHO = echo "going to compile for target $@"

OS := $(shell uname)
ifneq (,$(findstring CYGWIN_NT,$(OS)))
EXT = .exe
else
EXT =
endif

PROG = ex1_q1$(EXT)
all: $(PROG) test

$(PROG) : ex1_q1.o ex1_q1_functions.o
	$(CC) $(CFLAGS) ex1_q1.o ex1_q1_functions.o -o $(PROG)
	
clean:
	rm -vf *.o ${PROG} *.log
	
test:
	$(ECHO) going to run test...
	./$(PROG) < in.txt > out.log