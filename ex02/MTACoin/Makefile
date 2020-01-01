# Makefile for ex2-q1 winter 2019A
CFLAGS 		= -Wall
CC 			= gcc -std=c99
ZLIG_ADDON 	= -lz

OS := $(shell uname)
ifneq (,$(findstring CYGWIN_NT,$(OS)))
EXT = .exe
else
EXT =
endif

PROG = program$(EXT)

all: $(PROG) #$(HELPER) test

$(PROG) : Program.o Server.o Miscellaneous.o Miner.o Block.o
	@echo -n compiling $@ ... 
	@$(CC) $(CFLAGS) Program.o Server.o Miscellaneous.o Miner.o Block.o -o $(PROG) $(ZLIG_ADDON) -lm
	@echo  [OK]
	
clean:
	@echo removing files ...
	@rm -vf *.o ${PROG} *.log *.tmp ${HELPER}
	
test:
	@echo -n going to run test ...
	@./$(PROG) < in.txt > out.log
	@echo test finish [OK]