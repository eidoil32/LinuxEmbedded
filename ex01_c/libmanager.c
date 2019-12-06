#include "libmanager.h"

Block **blockArray = NULL;
int currentBlock, currentSize = 10;

extern Block* setBlock(char** arg) {
    Block *block = (Block*)malloc(sizeof(Block));
    block->parameters = arg;
    return block;
}

extern char* printBlock(Block *block) {
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
        if (i + 1 <= BLOCK_SIZE) strcat(blockDetails,"\n");
    }

    return blockDetails;
}

extern int convertToIndex(char *command) {
    if (!strcmp(command, COMMAND_BLOCK_BY_HASH))
        return 0;
    else if (!strcmp(command, COMMAND_BLOCK_BY_HEIGHT))
        return 1;
    return -1;
}

extern void addNewBlock(Block *block) {
    if (blockArray == NULL) {
        blockArray = (Block **) malloc(currentSize * sizeof(Block *));
        currentBlock = 0;
    } else if (currentBlock == currentSize) {
        currentSize *= 2;
        blockArray = (Block **) realloc(blockArray, currentSize * sizeof(Block *));
    }

    blockArray[currentBlock++] = block;
}

extern Block **readBlocks(int *numOfBlocks) {
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
        int systemResult = system(SHELL_SCRIPT_NAME);
        return readBlocks(numOfBlocks);
    }

    *numOfBlocks = currentBlock;
    return blockArray;
}

extern void printBlocks(Block **blocks, int size) {
    for (size_t i = 0; i < size; i++) {
        char *block = printBlock(blocks[i]);
        printf("%s", block);
        if (i + 1 != size)
            printf(BLOCK_SEPARATOR_ARROW);
        free(block);
    }
}

extern void readBlock(char **command, Block **blocks, int size) {
    int filter = convertToIndex(command[0]);
    if (filter != -1) {
        for (int i = 0; i < size; i++) {
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

extern int forceLoad() {
    FILE *db = fopen(BLOCK_DATABASE_NAME,"w"); // clear old database
    int systemResult = system(SHELL_SCRIPT_NAME); // rerun the script
    return 0;
}

extern int exportToCsv(Block **blocks, int size) {
    	FILE *fp = fopen(CSV_FILE,"w+");
	fprintf(fp,"Hash, Height, Total, Time, Relayed By, Prev Block");
	for (int i = 0; i < size; i++) {
		fprintf(fp,"\n%s",blocks[i]->parameters[0]);
		for (int j = 1; j < BLOCK_SIZE; j++) {
			fprintf(fp,",%s",blocks[i]->parameters[j]);
		}
	}
	fclose(fp);
	return 0;
}

extern bool file_exist (char *filename) {
    return fopen(filename, "r") != NULL;
}

extern char *toUpper(char *str) {
    char *s = str;
    for (; *s; s++)
        *s = (char)toupper(*s);

    return s;
}