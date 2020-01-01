#include "Block.h"
#include "Server.h"

BLOCK_T initBlock(int id) {
    BLOCK_T block = (BLOCK_T)malloc(BLOCK_T_SIZE);
    
    block->difficulty = BLOCK_INIT_DIFFICULY;
    block->height = block->prev_hash = block->hash = block->nonce = 0;
    block->timestamp = getCurrentTimeStamp();
    block->relayed_by = id;

    return block;
}

// crc32 function need calculate hash following fields of the block: 
// height, timestamp, prev_hash, nonce, relayed_by.
PartialBlock initPartialBlock(BLOCK_T block) {
    PartialBlock temp;

    temp.height     = block->height;
    temp.nonce      = block->nonce;
    temp.prev_hash  = block->prev_hash;
    temp.relayed_by = block->relayed_by;
    temp.timestamp  = block->timestamp;

    return temp;
}

unsigned int calcHash(BLOCK_T block) {
    PartialBlock temp = initPartialBlock(block);
    return crc32(0L, (Bytef*)&temp, sizeof(PartialBlock));
}