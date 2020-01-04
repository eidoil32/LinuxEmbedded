#include "miner.h"
#include "program.h"

void* miner_engine(void *miner_input) {
    Miner* miner = (Miner*)miner_input;

    while(true) {
        // getting last block from server;
        pthread_mutex_lock(&global_last_block_lock);
        BLOCK_T lastBlock = *global_last_block;
        pthread_mutex_unlock(&global_last_block_lock);

        BLOCK_T block = miner->create_block(miner->system_id, lastBlock);
        pthread_mutex_lock(&global_block_to_add_lock);
        if (global_block_to_add) { // if not NULL!
            pthread_cond_wait(&global_new_block_was_added, &global_block_to_add_lock);
        }

        global_block_to_add = copy_my_block(block);
        printf(MINER_MINED_NEW_BLOCK, miner->system_id, block.height, block.hash);
        pthread_mutex_unlock(&global_block_to_add_lock);
        pthread_cond_signal(&global_block_event);
    }
}

BLOCK_T* copy_my_block(BLOCK_T block) {
    BLOCK_T* ptr = (BLOCK_T*)malloc(sizeof(BLOCK_T));

    ptr->difficulty = block.difficulty;
    ptr->hash = block.hash;
    ptr->height = block.height;
    ptr->nonce = block.nonce;
    ptr->prev_hash = block.prev_hash;
    ptr->relayed_by = block.relayed_by;
    ptr->timestamp = block.timestamp;

    return ptr;
}

BLOCK_T calculate_block(int miner_id, BLOCK_T last) {
    BLOCK_T block = initBlock(miner_id, last.height, last.hash);
    PARTIAL_BLOCK_T partial_block = (PARTIAL_BLOCK_T){ block.height, block.timestamp, block.nonce, block.relayed_by, block.prev_hash };
    block.hash = crc32(0, (Bytef*)&partial_block, sizeof(PARTIAL_BLOCK_T));

    while ((block.hash & MASK_LAST_16_BITS) != 0) { //checking if the hash we got is with 16 zeros at the beginning
        partial_block.nonce = block.nonce = block.nonce + 1;
        block.hash = crc32(0, (Bytef*)&partial_block, sizeof(PARTIAL_BLOCK_T));
    }

    return block;
}

BLOCK_T fake_miner_block_creator(int miner_id, BLOCK_T last) {
    printf("in fake miner\n");
    sleep(1);
    BLOCK_T block = initBlock(miner_id, last.height, last.prev_hash);
    return block;
}