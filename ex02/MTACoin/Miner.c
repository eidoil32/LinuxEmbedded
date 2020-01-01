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
		printf("calculating new block.... miner %d",miner->id);
		block->nonce++;
		partialBlock.nonce = block->nonce;
		block->hash = crc32(0L, (Bytef*)&partialBlock, sizeof(PartialBlock));
	}

	return block;
}

void* minerEngine(void* inputPackage) {
	Package package = (Package)inputPackage;
	Miner miner = (Miner)package->parms[MINER_INDEX];
	StaticBlock* lastBlock = (StaticBlock*)package->parms[LAST_BLOCK_INDEX];
	BlockPackage* blockToAdd = (BlockPackage*)package->parms[BLOCK_TO_ADD_INDEX];

	while(true) {
		pthread_mutex_lock(miner->lastBlock_lock);
		BLOCK_T localLastBlock = **lastBlock;
		pthread_mutex_unlock(miner->lastBlock_lock);

		BLOCK_T newBlock = calculateNewBlock(miner, localLastBlock);
		pthread_mutex_lock(miner->blockToAdd_lock);
		if (*blockToAdd != NULL) {
			pthread_cond_wait(miner->newBlockWasAdded, miner->blockToAdd_lock);
			if (localLastBlock->height == newBlock->height) {
				pthread_mutex_unlock(miner->blockToAdd_lock);
				continue;
			}
		}

		(*blockToAdd) = (BlockPackage)calloc(1, BLOCK_PACKAGE_SIZE);
		(*blockToAdd)->creator = miner;
		(*blockToAdd)->newBlock = newBlock;
		printf(MINER_MINED_NEW_BLOCK, miner->id, newBlock->height, newBlock->hash);
		pthread_mutex_unlock(miner->blockToAdd_lock);
		pthread_cond_broadcast(miner->blockEvent);
	}
}