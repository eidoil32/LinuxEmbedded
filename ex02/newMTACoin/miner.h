#ifndef MTACOIN_MINER_H
#define MTACOIN_MINER_H 
#include "miscellaneous.h"

typedef struct miner {
    int system_id;
    pthread_t id;
    BLOCK_T (*create_block)(int, BLOCK_T);
} Miner;

void* miner_engine(void *miner_input);
BLOCK_T calculate_block(int miner_id, BLOCK_T last);
BLOCK_T* copy_my_block(BLOCK_T block);
BLOCK_T fake_miner_block_creator(int miner_id, BLOCK_T last);

#endif