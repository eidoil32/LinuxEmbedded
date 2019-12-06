#ifndef EX01_C_MAINCHAIN_H
#define EX01_C_MAINCHAIN_H

#include <ctype.h>
#include <stdlib.h>
#include "stdio.h"
#include "string.h"

typedef enum { false, true } bool;

typedef struct Block {
    char** parameters;
} Block;

// block array keys
#define BLOCK_SIZE              6
#define KEY_MAP_HASH            "hash:"
#define KEY_MAP_HEIGHT          "height:"
#define KEY_MAP_TOTAL           "total:"
#define KEY_MAP_TIME            "time:"
#define KEY_MAP_RELAYED_BY      "relayed_by:"
#define KEY_MAP_PREV_BLOCK      "prev_block:"
#define COMMAND_ARRAY_SIZE      2

// static array for keys
static char *blockKeys[BLOCK_SIZE] = { KEY_MAP_HASH, KEY_MAP_HEIGHT, KEY_MAP_TOTAL,
                                       KEY_MAP_TIME, KEY_MAP_RELAYED_BY, KEY_MAP_PREV_BLOCK };

// custom strings
#define CSV_FILE 				    "a.csv"
#define COMMAND_DELIMITER 			"-"
#define COMMAND_EXPORT_TO_CSV 		"CSV"
#define COMMAND_BLOCK_BY_HASH 		"HASH"
#define COMMAND_BLOCK_BY_HEIGHT 	"HEIGHT"
#define COMMAND_RELOAD_DATABASE 	"REL"
#define BLOCK_NOT_FOUND 			"error: block not found\n"
#define COMMAND_NOT_FOUND 			"error: the command '%s' not found\n"
#define UNKNOWN_ERROR   			"fatal error: unknown\n"
#define BLOCK_SEPARATOR_ARROW 			"\n\t\t\t\t\t|\n\t\t\t\t\t|\n\t\t\t\t\t|\n\t\t\t\t\tV\n"

// script shortcuts
#define SHELL_SCRIPT_NAME 			"./script.sh"
#define BLOCK_DATABASE_NAME 			"blockdb"

//functions
bool file_exist (char *filename);
char *toUpper(char *str);
void start(int argc, char* argv[]);
void addNewBlock(Block *block);
Block** readBlocks(int *numOfBlocks);
void printBlocks(Block** blocks, int size);
void readBlock(char **command, Block **blocks, int size);
int forceLoad();
int exportToCsv(Block** blocks, int size);
int convertToIndex(char *command);

#endif //EX01_C_MAINCHAIN_H
