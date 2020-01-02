#include "Miner.h"

Miner initMiner(int id) {
	Miner miner = (Miner)malloc(MINER_SIZE);
	miner->id = id;
	return miner;
}

BLOCK_T calculateNewBlock(Miner miner, BLOCK_T lastBlock) {
	BLOCK_T block = initBlock(miner->id);
	block->prev_hash = lastBlock->hash;
	block->height = lastBlock->height + 1;
	PartialBlock partialBlock = initPartialBlock(block);
	block->hash = crc32(0L, (Bytef*)&partialBlock, sizeof(PartialBlock));
	while (block->hash & MASK_LAST_16_BITS) {
		block->nonce++;
		partialBlock.nonce = block->nonce;
		block->hash = crc32(0L, (Bytef*)&partialBlock, sizeof(PartialBlock));
	}

	return block;
}

void* minerEngine(void* inputMiner) {
	Miner miner = (Miner)inputMiner;

	while(true) {
		pthread_mutex_lock(&global_lastBlock_lock);
		struct block_t localLastBlock = **global_lastBlock;
		pthread_mutex_unlock(&global_lastBlock_lock);

		BLOCK_T newBlock = calculateNewBlock(miner, &localLastBlock);
		pthread_mutex_lock(&global_blockToAdd_lock);
		if (global_blockToAdd != NULL) {
			pthread_cond_wait(&global_newBlockWasAdded, &global_blockToAdd_lock);
			continue; // new block was added, so we check if it's updated and calculate new one;
		}

		global_blockToAdd = &newBlock;

		printf(MINER_MINED_NEW_BLOCK, miner->id, newBlock->height, newBlock->hash);

		pthread_mutex_unlock(&global_blockToAdd_lock);
		pthread_cond_broadcast(&global_blockEvent);
	}
}