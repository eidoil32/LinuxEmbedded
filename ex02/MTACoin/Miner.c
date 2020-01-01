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

	printf("Miner %d is calculated new block: 0x%x and finish!\n",miner->id, block->hash);
	return block;
}

void* minerEngine(void* inputPackage) {
	Package package = (Package)inputPackage;
	Miner miner = (Miner)package->parms[MINER_INDEX];
	StaticBlock* lastBlock = (StaticBlock*)package->parms[LAST_BLOCK_INDEX];
	Package blockToAdd = (Package)package->parms[BLOCK_TO_ADD_INDEX];

	while(true) {
		pthread_mutex_lock(miner->lastBlock_lock); // for checking if lastBlock is NULL
		if (!(**lastBlock)) {
			pthread_cond_wait(miner->newBlockWasAdded, miner->lastBlock_lock);
		}

		BLOCK_T localLastBlock = **lastBlock;
		pthread_mutex_unlock(miner->lastBlock_lock);
		BLOCK_T newBlock = calculateNewBlock(miner, localLastBlock);
		pthread_mutex_lock(miner->blockToAdd_lock);
		if (blockToAdd->parms[NEW_BLOCK_BLOCK_INDEX] != NULL) {
			pthread_cond_wait(miner->newBlockWasAdded, miner->blockToAdd_lock);
		}

		blockToAdd->parms[NEW_BLOCK_BLOCK_INDEX] = newBlock;
		pthread_mutex_unlock(miner->blockToAdd_lock);
		pthread_cond_broadcast(miner->newBlockWasAdded);
	}
}