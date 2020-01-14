#ifndef MTACOIN_MINER_H
#define MTACOIN_MINER_H 
#include "miscellaneous.h"

typedef struct miner {
    int miner_id;
} Miner;

void miner_engine();
BLOCK_T calculate_block(int miner_id, BLOCK_T last);
BLOCK_T* copy_my_block(BLOCK_T block);

#define MQ_MINER_MAX_MESSAGES                       1
#define MQ_MINER_MAX_MSG_SIZE                       sizeof(BLOCK_T)
#define MQ_SERVER_MAX_MESSAGES                      10
#define MQ_SERVER_MAX_MSG_SIZE                      sizeof(Miner)

#endif