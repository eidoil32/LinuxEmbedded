#include <stdio.h>
#include "libmanager.h"

void start(int argc, char* argv[]);

int main(int argc, char **argv) {
	start(argc, argv);
	return 0;
}

void start(int argc, char* argv[]) {
    int numOfBlocks = 0;
    Block **blocks = readBlocks(&numOfBlocks);

    exportToCsv(blocks,numOfBlocks);

    for (int i = 0; i < numOfBlocks; ++i)
        free(blocks[i]);
    free(blocks);
}