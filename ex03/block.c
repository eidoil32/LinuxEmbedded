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