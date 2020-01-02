#ifndef MTACOIN_SERVER_H
#define	MTACOIN_SERVER_H
#include "Miscellaneous.h"
#include "Block.h"
#include "Miner.h"

//server id = -1 in first block;
struct server { 
	pthread_t threadID;
	List blocks;
};

// for easier readability
typedef struct server* 	Server;
#define SERVER_SIZE		sizeof(struct server)

Server initServer();
bool checkBlock(BLOCK_T block);
bool approveBlock(BLOCK_T block, List blocks);
void *serverEngine(void* inputPackage);
BLOCK_T createFirstBlock();
#endif