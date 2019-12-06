#include <stdio.h>
#include "libmanager.h"

enum Commands { FILTER_BY_HASH, FILTER_BY_HEIGHT, EXPORT_TO_CSV, RELOAD_BASE, PRINT_ALL, UNKNOWN, ERROR};

#define ILLEAGLE_COMMAND 			printf("error: illegal command\n")
#define COMMAND_CANNOT_BE_WITHOUT_PARAMETER 	printf("error: must insert parameter to this command\n")
#define EMPTY_PARAMETER_BLOCK 			if (argc == 2) { COMMAND_CANNOT_BE_WITHOUT_PARAMETER; return ERROR; }
void start(int argc, char* argv[]);
int getCommand(int argc, char *argv[], char **command);

int main(int argc, char **argv) {
	start(argc, argv);
	return 0;
}

int getCommand(int argc, char *argv[], char **command) {
    if (argc == 1) {
        return PRINT_ALL;
    } else {
	command[0] = strtok(argv[1], COMMAND_DELIMITER);
	toUpper(command[0]);
	if (!strcmp(command[0], COMMAND_BLOCK_BY_HASH)) {
        command[1] = argv[2];
		EMPTY_PARAMETER_BLOCK
		else return FILTER_BY_HASH;
	} else if (!strcmp(command[0], COMMAND_BLOCK_BY_HEIGHT)) {
        command[1] = argv[2];
		EMPTY_PARAMETER_BLOCK
		else return FILTER_BY_HEIGHT;
    	}
    }	

    return UNKNOWN;
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
	case ERROR:
	    break;
        case UNKNOWN:
        default:
            printf(COMMAND_NOT_FOUND, argv[1]);
            break;
    }

    if (result == -1)
        printf(UNKNOWN_ERROR);

    for (int i = 0; i < numOfBlocks; ++i)
        free(blocks[i]);
    free(blocks);
}
