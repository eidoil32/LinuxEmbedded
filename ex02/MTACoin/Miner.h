#ifndef MTACOIN_MINER_H
#define	MTACOIN_MINER_H
#include "Miscellaneous.h"
#include "Block.h"

struct miner {
	int id;
	pthread_t threadID;
};

// for easier readability
typedef struct miner* 	Miner;
typedef List 			Miners;

#define MINER_SIZE		sizeof(struct miner)

Miner initMiner(int id);
void* minerEngine(void* inputPackage);
BLOCK_T calculateNewBlock(Miner miner, BLOCK_T lastBlock);

#endif