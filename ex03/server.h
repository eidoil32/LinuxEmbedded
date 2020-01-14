#ifndef MTACOIN_SERVER_H
#define MTACOIN_SERVER_H 
#include "miscellaneous.h"
#include "miner.h"


typedef struct mq_attr mq_attributes;

typedef struct server {
    mqd_t server_mq;
    mqd_t *miners_mq;
    mq_attributes **miners_attr;
    int miners_mq_logical_size, miners_mq_physical_size;
    List blocks;
} Server;

void approve_block(BLOCK_T *block);
void server_engine();
void create_first_block();
void checking_for_new_miners(struct mq_attr *attr);
void add_miner_to_mq_array(Miner miner_mq);

#endif