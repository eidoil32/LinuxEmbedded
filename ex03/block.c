#include "block.h"
#include "miscellaneous.h"

BLOCK_T initBlock(int minerID, int height, unsigned int prev_hash) {
    BLOCK_T block = (BLOCK_T)INITIALIZE_BLOCK_T;
    block.relayed_by = minerID;
    block.height = height + 1;
    block.prev_hash = prev_hash;
    block.timestamp = getCurrentTimeStamp();

    return block;
}

PARTIAL_BLOCK_T create_partial_block(BLOCK_T *block) {
    return (PARTIAL_BLOCK_T){ block->height, block->timestamp, block->nonce, block->relayed_by, block->prev_hash };
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
