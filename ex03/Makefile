# Makefile for ex3
CFLAGS 			= -Wall
CC 				= gcc -std=c99
ZLIG_ADDON 		= -lz
current_dir 	= $(shell pwd) #find current directory
SUPPORT_LIBS 	= -L=$(current_dir) -lmiscellaneous -lblock -Wl,-rpath=$(current_dir)

OS := $(shell uname)

# applications name
PROG 			= launcher
MINER 			= miner
SERVER 			= server
MISCELLANEOUS 	= libmiscellaneous.so
LIBBLOCK		= libblock.so

# creating miscellaneous as .SO package and create programs for miners, server and launcher
all: $(MISCELLANEOUS) $(LIBBLOCK) $(MINER) $(SERVER) $(PROG)

$(MISCELLANEOUS): miscellaneous.c 
	@echo -n compiling .SO file for miscellaneous $@ ... 
	@$(CC) -shared -fPIC -o $(MISCELLANEOUS) miscellaneous.c
	@echo [OK]

$(LIBBLOCK): block.c
	@echo -n compiling .SO file for block $@ ... 
	@$(CC) -shared -fPIC -o $(LIBBLOCK) block.c -L=$(current_dir) -lmiscellaneous -Wl,-rpath=$(current_dir)
	@echo [OK]

$(MINER): $(MISCELLANEOUS) $(LIBBLOCK) miner.o
	@echo -n compiling miner $@ ... 
	@$(CC) $(CFLAGS) miner.o -o $(MINER) $(ZLIG_ADDON) -lm $(SUPPORT_LIBS) -lrt
	@echo  [OK]

$(SERVER): $(MISCELLANEOUS) $(LIBBLOCK) server.o 
	@echo -n compiling server $@ ... 
	@$(CC) $(CFLAGS) server.o -o $(SERVER) $(ZLIG_ADDON) -lm $(SUPPORT_LIBS) -lrt
	@echo  [OK]

$(PROG): $(MISCELLANEOUS) $(LIBBLOCK) launcher.o 
	@echo -n compiling $@ ... 
	@$(CC) $(CFLAGS) launcher.o -o $(PROG) $(ZLIG_ADDON) -lm $(SUPPORT_LIBS) -lrt
	@echo  [OK]
	
clean:
	@echo removing files..
	@rm -vf *.o $(MISCELLANEOUS) $(LIBBLOCK) $(MINER) $(SERVER) $(PROG) *.log *.tmp *.so 
	@echo all files removed successfully