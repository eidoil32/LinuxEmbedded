#include "Server.h"

Server initServer() {
	Server server = (Server)calloc(1, SERVER_SIZE);
	server->blocks = (List)calloc(1, LIST_SIZE);
	return server;
}

bool checkBlock(BLOCK_T block) {
	return (!(block->hash & MASK_LAST_16_BITS)) && (block->hash == calcHash(block));
}	//   if block->hash & 0xFFFF0000 == 0 ^

bool approveBlock(BLOCK_T block, List blocks) {
	BLOCK_T last = (BLOCK_T)blocks->tail->data;
	if (checkBlock(block)) {
		if (last->hash == block->prev_hash) {
			printf(SERVER_APPROVED_BLOCK_BY_MINER, block->relayed_by, block->height, block->timestamp, 
					block->hash, block->prev_hash, block->difficulty, block->nonce);
			addNode(blocks, block);
			return true;
		}
	}

	printf(SERVER_WRONG_BLOCK, block->height, block->relayed_by, block->hash, calcHash(block));
	return false;
}

BLOCK_T createFirstBlock() {
	BLOCK_T block = initBlock(-1);
	
	block->hash = calcHash(block);
    while(!checkBlock(block)) {
        ++(block->nonce);
        block->hash = calcHash(block);
    }

	return block;
}

void *serverEngine(void *inputServer) {
	Server server = (Server)inputServer;

	while(true) {
		pthread_mutex_lock(&global_blockToAdd_lock);
		if (!(*global_blockToAdd)) {
			pthread_cond_wait(&global_blockEvent, &global_blockToAdd_lock);
		}

		BLOCK_T block = copyCtor(*global_blockToAdd);
		*global_blockToAdd = NULL;
		pthread_mutex_unlock(&global_blockToAdd_lock);  // prevent deadlock (?) or should be in the end of this section
		pthread_cond_broadcast(&global_newBlockWasAdded);
		if(approveBlock(block, server->blocks)) { // update lastBlock to point on the new block
			pthread_mutex_lock(&global_lastBlock_lock);
			global_lastBlock = &block;
			pthread_mutex_unlock(&global_lastBlock_lock);
			pthread_cond_broadcast(&global_newBlockWasAdded); 	/* notify miners that new block was 
																added and now they need to calculate new one */
		}
	}


	return NULL;
}