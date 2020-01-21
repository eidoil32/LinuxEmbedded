#ifndef MTACOIN_MINER_H
#define MTACOIN_MINER_H 
#include "miscellaneous.h"

typedef struct miner {
    int miner_id;
} Miner;

void miner_engine();
BLOCK_T calculate_block(int miner_id, BLOCK_T last);

#endif