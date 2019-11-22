//
// Created by ido on 21/11/2019.
//

#ifndef EX01_C_BLOCK_H
#define EX01_C_BLOCK_H
#include "stdio.h"
#include <string.h>
#include "Miscellaneous.h"

typedef struct Block {
    char **parameters;
} Block;

Block* setBlock(char **arg);
char* printBlock(Block* block);
#endif //EX01_C_BLOCK_H
