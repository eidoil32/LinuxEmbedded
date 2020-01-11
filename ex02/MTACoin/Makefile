CFLAGS 		= -Wall
CC 			= gcc -std=c99 -ggdb
ZLIG_ADDON 	= -lz

OS := $(shell uname)

PROG = program

all: $(PROG) 

$(PROG) : program.o server.o miscellaneous.o miner.o block.o
	@echo -n compiling $@ ... 
	@$(CC) $(CFLAGS) -pthread program.o server.o miscellaneous.o miner.o block.o -o $(PROG) $(ZLIG_ADDON) -lm
	@echo  [OK]
	
clean:
	@echo removing files ...
	@rm -vf *.o ${PROG} *.log *.tmp ${HELPER}