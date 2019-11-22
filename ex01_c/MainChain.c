//
// Created by ido on 21/11/2019.
//

#include "MainChain.h"

Block **blockArray = NULL;
int currentBlock, currentSize = 10;

int getCommand(int argc, char *argv[], char **command) {
    command[0] = toUpper(strtok(argv[1], COMMAND_DELIMITER));
    if (argc == 1) {
        return PRINT_ALL;
    } else if (argc == 2) {
        if (!strcmp(command[0], COMMAND_EXPORT_TO_CSV))
            return EXPORT_TO_CSV;
    } else {
        command[1] = argv[1];
        if (!strcmp(command[0], COMMAND_BLOCK_BY_HASH))
            return FILTER_BY_HASH;
        else if (!strcmp(command[0], COMMAND_BLOCK_BY_HEIGHT))
            return FILTER_BY_HEIGHT;
        else if (!strcmp(command[0], COMMAND_RELOAD_DATABASE))
            return RELOAD_BASE;
    }

    return UNKNOWN;
}

int convertToIndex(char *command) {
    if (!strcmp(command, COMMAND_BLOCK_BY_HASH))
        return 0;
    else if (!strcmp(command, COMMAND_BLOCK_BY_HEIGHT))
        return 1;
    return -1;
}

void start(int argc, char* argv[]) {
    int numOfBlocks = 0, commandNumber = UNKNOWN, result = 0;
    Block **blocks = readBlocks(&numOfBlocks);
    char **command = (char**)malloc(COMMAND_ARRAY_SIZE*sizeof(char*));
    commandNumber = getCommand(argc, argv, command);
    switch (commandNumber) {
        case FILTER_BY_HASH:
        case FILTER_BY_HEIGHT:
            readBlock(command,blocks,numOfBlocks);
            break;
        case EXPORT_TO_CSV:
            result = exportToCsv(blocks,numOfBlocks);
            break;
        case RELOAD_BASE:
            result = forceLoad();
            break;
        case PRINT_ALL:
            printBlocks(blocks,numOfBlocks);
            break;
        case UNKNOWN:
        default:
            printf(COMMAND_NOT_FOUND, command[0]);
            break;
    }

    if (result == -1)
        printf(UNKNOWN_ERROR);

    for (int i = 0; i < numOfBlocks; ++i)
        free(blocks[i]);
    free(blocks);
}

void addNewBlock(Block *block) {
    if (blockArray == NULL) {
        blockArray = (Block **) malloc(currentSize * sizeof(Block *));
        currentBlock = 0;
    } else if (currentBlock == currentSize) {
        currentSize *= 2;
        blockArray = (Block **) realloc(blockArray, currentSize * sizeof(Block *));
    }

    blockArray[currentBlock++] = block;
}

Block **readBlocks(int *numOfBlocks) {
    if (file_exist(BLOCK_DATABASE_NAME)) {
        FILE *blockDB = fopen(BLOCK_DATABASE_NAME, "r");
        char *line = NULL;
        size_t read, len = 0;

        while ((read = getline(&line, &len, blockDB)) != -1) {
            char **args = (char **) malloc(BLOCK_SIZE * sizeof(char *));
            for (size_t i = 0; i < BLOCK_SIZE; i++, (i + 1 <= BLOCK_SIZE ? getline(&line, &len, blockDB) : 1)) {
                args[i] = (char *) malloc(strlen(line) + 1);
                strtok(line, " ");
                strcpy(args[i], strtok(NULL, "\n"));
            }
            addNewBlock(setBlock(args));
        }

        fclose(blockDB);
    } else {
        system(SHELL_SCRIPT_NAME);
        return readBlocks(numOfBlocks);
    }

    *numOfBlocks = currentBlock;
    return blockArray;
}

void printBlocks(Block **blocks, int size) {
    for (size_t i = 0; i < size; i++) {
        char *block = printBlock(blocks[i]);
        printf("%s", block);
        if (i + 1 != size)
            printf(BLOCK_SEPARATOR);
        free(block);
    }
}

void readBlock(char **command, Block **blocks, int size) {
    int filter = convertToIndex(command[0]);
    if (filter != -1) {
        for (size_t i = 0; i < size; i++) {
            Block *current = blocks[i];
            if (!strcmp(current->parameters[filter], command[1])) {
                char *block = printBlock(current);
                printf("%s", block);
                free(block);
                return;
            }
        }
    }

    printf(BLOCK_NOT_FOUND);
}

int forceLoad() {
    fopen(BLOCK_DATABASE_NAME,"w"); // clear old database
    system(SHELL_SCRIPT_NAME); // rerun the script

    return 0;
}

int exportToCsv(Block **blocks, int size) {
    return -1;
}