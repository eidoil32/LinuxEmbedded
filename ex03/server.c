#include "server.h"
#include "launcher.h"

Server server;

int main(int argc, char* argv[]) {
    server_engine(); // server engine start..
    return 0;
}

void server_engine() {
    initList(&server.blocks);
    
    mq_attributes attr = { 0 };
    attr.mq_maxmsg  = MQ_SERVER_MAX_MESSAGES;
    attr.mq_msgsize = MQ_SERVER_MAX_MSG_SIZE;

    server.miners_mq_logical_size = MINERS_MQ_ARRAY_SIZE;
    server.miners_mq_physical_size = 0;
    server.miners_mq = (mqd_t*)malloc(MINERS_MQ_ARRAY_SIZE * sizeof(mqd_t));

    mq_unlink(SERVER_MESSAGE_QUEUE);
    server.server_mq = mq_open(SERVER_MESSAGE_QUEUE, O_CREAT, S_IRWXU | S_IRWXG, &attr);
    printf(SERVER_LISTENING_MSG_QUEUE, SERVER_MESSAGE_QUEUE);

    create_first_block();

    while (true) {
        checking_for_new_miners(&attr);
        checking_for_new_blocks();
    }
}

void send_to_all_miners_new_block() {
    for (size_t i = 0; i < server.miners_mq_physical_size; ++i) {
        mq_attributes attr;
        mq_getattr(server.miners_mq[i], &attr);
        if (attr.mq_curmsgs > 0) {
            BLOCK_T new_block;
            mq_receive(server.miners_mq[i], (char*)&new_block, MQ_MINER_MAX_MSG_SIZE, NULL); // empty queue for new block
        }
        mq_send(server.miners_mq[i], (char*)&server.blocks.tail->block, MQ_MINER_MAX_MSG_SIZE, 0); // send to miner the last block
    }
}

void checking_for_new_miners(mq_attributes *attr) {
    mq_getattr(server.server_mq, attr); 
    while (attr->mq_curmsgs > 0) {
        Miner miner;
        mq_receive(server.server_mq, (char*)&miner, MQ_SERVER_MAX_MSG_SIZE, NULL);
        add_miner_to_mq_array(miner);
        mq_getattr(server.server_mq, attr); 
    }
}

void checking_for_new_blocks() {
    for (size_t i = 0; i < server.miners_mq_physical_size; ++i) {
        mq_attributes attr;
        mq_getattr(server.miners_mq[i], &attr);
        if (attr.mq_curmsgs > 0) {
            BLOCK_T new_block;
            mq_receive(server.miners_mq[i], (char*)&new_block, MQ_MINER_MAX_MSG_SIZE, NULL);
            if (approve_block(&new_block)) {
                send_to_all_miners_new_block();
            } /* else {
                mq_send(server.miners_mq[i], (char*)&server.blocks.tail->block, MQ_MINER_MAX_MSG_SIZE, 0)
            } */
        }
    }
}

void add_miner_to_mq_array(Miner miner) {
    char *miner_mq_name = (char*)calloc(strlen(MINER_MESSAGE_QUEUE_PREFIX) + MAX_SUPPORTED_NUM_OF_DIGITS + 1, sizeof(char));
    sprintf(miner_mq_name, "%s%d", MINER_MESSAGE_QUEUE_PREFIX, miner.miner_id);
    printf(RECEIVED_NEW_MINER_CONNECTION, miner.miner_id, miner_mq_name);
    mqd_t miner_mq = mq_open(miner_mq_name, O_RDWR);

    if (server.miners_mq_physical_size == server.miners_mq_logical_size) {
        server.miners_mq_logical_size *= 2;
        server.miners_mq = (mqd_t*)realloc(server.miners_mq, sizeof(mqd_t)*(server.miners_mq_logical_size));
    } 

    if (mq_send(miner_mq, (char*)&server.blocks.tail->block, MQ_MINER_MAX_MSG_SIZE, 0) != 0) // send to miner the last block
        fprintf(stderr, SEND_BLOCK_TO_MINER_FAILED, miner.miner_id);        

    server.miners_mq[server.miners_mq_physical_size++] = miner_mq;
    free(miner_mq_name);
}

void create_first_block() {
    BLOCK_T block = initBlock(0, -1, 0);
    PARTIAL_BLOCK_T partial_block = create_partial_block(&block);
    do {
        partial_block.nonce = block.nonce = block.nonce + 1;
        block.hash = crc32(0, (Bytef*)&partial_block, sizeof(PARTIAL_BLOCK_T));
    } while ((block.hash & MASK_LAST_16_BITS) != 0);

    addNode(&server.blocks, block);
}

bool approve_block(BLOCK_T *block) {
    BLOCK_T prev_block_hash = server.blocks.tail->block;
    PARTIAL_BLOCK_T partial_block = create_partial_block(block);
    unsigned int hash = crc32(0L, (Bytef*)&partial_block, sizeof(PARTIAL_BLOCK_T));

    if (block->prev_hash == prev_block_hash.hash) {
        if (block->height == (prev_block_hash.height + 1)) {
            if (hash == block->hash) {
                addNode(&server.blocks, *block);
                printf(SERVER_APPROVED_BLOCK_BY_MINER,  block->relayed_by,
                                                        block->height, 
                                                        block->timestamp,
                                                        block->hash,
                                                        block->prev_hash,
                                                        block->difficulty,
                                                        block->nonce);
                return true;
            }
        }
    }

    printf(SERVER_WRONG_BLOCK, block->height, block->relayed_by, block->hash, hash);
    return false;
}