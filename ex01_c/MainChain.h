//
// Created by ido on 21/11/2019.
//

#ifndef EX01_C_MAINCHAIN_H
#define EX01_C_MAINCHAIN_H
#include "stdio.h"
#include "string.h"
#include "Miscellaneous.h"
#include "Block.h"

enum Commands { FILTER_BY_HASH, FILTER_BY_HEIGHT, EXPORT_TO_CSV, RELOAD_BASE, PRINT_ALL, UNKNOWN};

#define COMMAND_ARRAY_SIZE 2

void start(int argc, char* argv[]);
void addNewBlock(Block *block);
Block** readBlocks(int *numOfBlocks);
void printBlocks(Block** blocks, int size);
void readBlock(char **command, Block **blocks, int size);
int forceLoad();
int exportToCsv(Block** blocks, int size);
#endif //EX01_C_MAINCHAIN_H
