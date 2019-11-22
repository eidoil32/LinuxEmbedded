//
// Created by ido on 21/11/2019.
//
#include "Block.h"
Block* setBlock(char** arg) {
    Block *block = (Block*)malloc(sizeof(Block));
    block->parameters = arg;
    return block;
}

/*
char* printBlock(Block *block) {
    unsigned long blockDetailsSize = 0;
    char **blockDetails = (char**)malloc(BLOCK_SIZE*sizeof(char*));
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        unsigned long parameterLength = strlen(block->parameters[i]) + 2;
        blockDetailsSize += parameterLength;
        char *buffer = (char*)malloc(parameterLength);
        snprintf(buffer, sizeof(buffer), "%s %s", blockKeys[i], block->parameters[i]);
        blockDetails[i] = buffer;
    }

    char *result = (char*)malloc(blockDetailsSize);
    snprintf(result,sizeof(result),BLOCK_FORMAT,BLOCK_PARAMETERS_ARRAY(blockDetails));

    for (int j = 0; j < BLOCK_SIZE; ++j) {
        free(blockDetails[j]);
    }
    free(blockDetails);

    return result;
}*/

char* printBlock(Block *block) {
    unsigned long size = strlen(blockKeys[0]) + strlen(block->parameters[0]) + 2;
    char *blockDetails = (char*)calloc(size,sizeof(char));

    for (int i = 0; i < BLOCK_SIZE; ++i) {
        unsigned long parameterSize = strlen(blockKeys[i]) + strlen(block->parameters[i]) + 2;
        if (strlen(blockDetails) + parameterSize > size) {
            size += parameterSize;
            blockDetails = (char*)realloc(blockDetails,size);
        }
        strcat(blockDetails,blockKeys[i]);
        strcat(blockDetails," ");
        strcat(blockDetails,block->parameters[i]);
        if (i + 1 < BLOCK_SIZE) strcat(blockDetails,"\n");
    }

    return blockDetails;
}
