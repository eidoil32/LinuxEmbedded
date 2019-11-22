//
// Created by ido on 21/11/2019.
//

#ifndef EX01_C_MISCELLANEOUS_H
#define EX01_C_MISCELLANEOUS_H
#include "stdio.h"
#include <stdlib.h>

// blockArray keys
#define BLOCK_SIZE 6
#define KEY_MAP_HASH            "hash:"
#define KEY_MAP_HEIGHT          "height:"
#define KEY_MAP_TOTAL           "total:"
#define KEY_MAP_TIME            "time:"
#define KEY_MAP_RELAYED_BY      "relayed_by:"
#define KEY_MAP_PREV_BLOCK      "prev_block:"
#define KEY_MAP_HASH_INDEX       0
#define KEY_MAP_HEIGHT_INDEX     1
#define KEY_MAP_TOTAL_INDEX      2
#define KEY_MAP_TIME_INDEX       3
#define KEY_MAP_RELAYED_BY_INDEX 4
#define KEY_MAP_PREV_BLOCK_INDEX 5

static char *blockKeys[BLOCK_SIZE] = { KEY_MAP_HASH, KEY_MAP_HEIGHT, KEY_MAP_TOTAL,
                                       KEY_MAP_TIME, KEY_MAP_RELAYED_BY, KEY_MAP_PREV_BLOCK };

typedef enum { false, true } bool;

// custom strings
#define BLOCK_FORMAT "%s\n%s\n%s\n%s\n%s\n%s"
#define BLOCK_PARAMETERS_ARRAY(array) array[0],array[1],array[2],array[3],array[4],array[5]
#define UNKNOWN_PARAMETER_SIGN "-1"
#define WHITE_SPACE " "
#define COMMAND_DELIMITER "-"
#define EMPTY_PARAMETER "error: not parameter is not entered"
#define COMMAND_EXPORT_TO_CSV "CSV"
#define COMMAND_BLOCK_BY_HASH "HASH"
#define COMMAND_BLOCK_BY_HEIGHT "HEIGHT"
#define COMMAND_RELOAD_DATABASE "REL"
#define BLOCK_NOT_FOUND "error: block not found\n"
#define COMMAND_NOT_FOUND "error: the command '%s' not found\n"
#define UNKNOWN_ERROR   "fatal error: unknown\n"
#define BLOCK_SEPARATOR "\n\t\t\t\t\t|\n\t\t\t\t\t|\n\t\t\t\t\t|\n\t\t\t\t\tV\n"

// exception strings
#define EXCEPTION_STR_BLOCK_NOT_VALID "error: Block not contain all information"

// script
#define SHELL_SCRIPT_NAME "./script"
#define BLOCK_DATABASE_NAME "blockdb"

//functions
bool file_exist (char *filename);
char *toUpper(char *str);
#endif //EX01_C_MISCELLANEOUS_H
