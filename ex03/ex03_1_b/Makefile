# Makefile for ex3-q1 winter 2019A
CFLAGS = -Wall
CC = gcc -std=c99

OS := $(shell uname)
ifneq (,$(findstring CYGWIN_NT,$(OS)))
EXT = .exe
else
EXT =
endif

PROG = ex3_q1$(EXT)
SUBST = subst$(EXT)

all: $(PROG) $(SUBST) test
	
$(SUBST) : subst.o
	@echo -n compiling subst program $@ ... 
	@$(CC) $(CFLAGS) subst.o -o $(SUBST)
	@echo  [OK]
	
$(PROG) : ex3_q1.o
	@echo -n compiling $@ ... 
	@$(CC) $(CFLAGS) ex3_q1.o -o $(PROG)
	@echo  [OK]
	
clean:
	@echo removing files ...
	@rm -vf *.o ${PROG} *.log *.tmp ${SUBST}
	
test:
	@echo -n going to run test ...
	@./$(PROG) abc xyz out.log < in.txt 2> err.log
	@echo test finish [OK]