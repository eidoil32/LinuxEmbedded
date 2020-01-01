#include "Server.h"

Server initServer() {
	Server server = (Server)calloc(1, SERVER_SIZE);
	return server;
}

bool checkBlock(BLOCK_T block) {
	return (block->hash & MASK_LAST_16_BITS) && (block->hash == calcHash(block));
}	//   if block->hash & 0xFFFF0000 == 0 ^

bool approveBlock(BLOCK_T block, Miner miner, List blocks) {
	if (checkBlock(block)) {
		BLOCK_T last = (BLOCK_T)blocks->tail->data;
		if (last->hash == block->prev_hash) {
			printf(SERVER_APPROVED_BLOCK_BY_MINER, miner->id, block->height, block->timestamp, 
					block->hash, block->prev_hash, block->difficulty, block->nonce);
			addNode(blocks, block);
			return true;
		}
	}

	printf(SERVER_WRONG_BLOCK, block->height, miner->id, block->hash, calcHash(block));
	return false;
}

BLOCK_T createFirstBlock() {
	BLOCK_T block = initBlock(-1);
	
	PartialBlock temp = initPartialBlock(block);
	block->hash = crc32(0L, (Bytef*)&temp, sizeof(PartialBlock));
    while(!checkBlock(block)) {
        ++(block->nonce);
		PartialBlock temp = initPartialBlock(block);
        block->hash = crc32(0L, (Bytef*)&temp, sizeof(PartialBlock));
    }

	return block;
}

void *serverEngine(void *inputPackage) {
	Package package = (Package)inputPackage;
	Server server = (Server)package->parms[SERVER_INDEX];
	StaticBlock lastBlock = (StaticBlock)package->parms[LAST_BLOCK_INDEX];
	Package blockToAdd = (Package)package->parms[BLOCK_TO_ADD_INDEX];
				
	addNode(server->blocks, createFirstBlock()); //creating the first block

	pthread_mutex_lock(&server->lastBlock_lock);
	*lastBlock = server->blocks->head->data;
	pthread_mutex_unlock(&server->lastBlock_lock);
	pthread_cond_broadcast(&server->newBlockWasAdded);

	while(true) {
		pthread_mutex_lock(&server->blockToAdd_lock);
		if (blockToAdd->parms[NEW_BLOCK_BLOCK_INDEX] == NULL) {
			pthread_cond_wait(&server->blockEvent, &server->blockToAdd_lock);
		}

		BLOCK_T block = blockToAdd->parms[NEW_BLOCK_BLOCK_INDEX];
		Miner miner = blockToAdd->parms[NEW_BLOCK_MINER_INDEX];
		blockToAdd->parms[NEW_BLOCK_BLOCK_INDEX] = NULL; // make it NULL for other miners to try
		pthread_mutex_unlock(&server->blockToAdd_lock);  // prevent deadlock (?) or should be in the end of this section
		if(approveBlock(block, miner, server->blocks)) { // update lastBlock to point on the new block
			pthread_mutex_lock(&server->lastBlock_lock);
			*lastBlock = block;
			pthread_mutex_unlock(&server->lastBlock_lock);
			pthread_cond_broadcast(&server->newBlockWasAdded); 	/* notify miners that new block was 
																	added and now they need to calculate new one */
		}
		pthread_cond_broadcast(&server->newBlockWasAdded); 	// notify miners that they can send blocks
	}


	return NULL;
}
