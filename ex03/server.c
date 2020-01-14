#include "server.h"
#include "launcher.h"

Server server;

int main(int argc, char* argv[]) {
    server_engine(); // server engine start..
    return 0;
}

void server_engine() {
    initList(&server.blocks);
    create_first_block();
    
    mq_attributes attr = { 0 };
    attr.mq_maxmsg  = MQ_SERVER_MAX_MESSAGES;
    attr.mq_msgsize = MQ_SERVER_MAX_MSG_SIZE;

    mq_unlink(SERVER_MESSAGE_QUEUE);

    server.server_mq = mq_open(SERVER_MESSAGE_QUEUE, O_CREAT, S_IRWXU | S_IRWXG, &attr);
    printf(SERVER_LISTENING_MSG_QUEUE, SERVER_MESSAGE_QUEUE);

    while (true) {
        checking_for_new_miners(&attr);
        
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

void add_miner_to_mq_array(Miner miner) {
    char *miner_mq_name = (char*)calloc(strlen(MINER_MESSAGE_QUEUE_PREFIX) + MAX_SUPPORTED_NUM_OF_DIGITS + 1, sizeof(char));
    sprintf(miner_mq_name, "%s%d", MINER_MESSAGE_QUEUE_PREFIX, miner.miner_id);
    mq_attributes* attr = (mq_attributes*)calloc(1, sizeof(mq_attributes));
    mqd_t miner_mq = mq_open(miner_mq_name, 0, S_IRWXU | S_IRWXG, attr);

    if (server.miners_mq_physical_size == server.miners_mq_logical_size) {
        server.miners_mq_logical_size *= 2;
        server.miners_mq = (mqd_t*)realloc(server.miners_mq, sizeof(mqd_t)*(server.miners_mq_logical_size));
        server.miners_attr = (mq_attributes**)realloc(server.miners_mq, sizeof(mq_attributes*)*(server.miners_mq_logical_size));
    } 

    mq_send(miner_mq, (char*)&server.blocks.tail->block, MQ_SERVER_MAX_MSG_SIZE, NULL); // send to miner the last block

    server.miners_mq[server.miners_mq_physical_size++] = miner_mq;
    server.miners_attr[server.miners_mq_physical_size - 1] = attr;
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

void approve_block(BLOCK_T *block) {
    BLOCK_T prev_block_hash = server.blocks.tail->block;
    PARTIAL_BLOCK_T partial_block = create_partial_block(block);
    unsigned int hash = crc32(0L, (Bytef*)&partial_block, sizeof(PARTIAL_BLOCK_T));

    if (block->prev_hash == prev_block_hash.hash) {
        if (block->height == (prev_block_hash.height + 1)) {
            if (hash == block->hash) {
                // locking 'global_last_block_lock' to add an new block to list (so other miners will mine new block with the correct parameters)
                addNode(&server.blocks, *block);
                printf(SERVER_APPROVED_BLOCK_BY_MINER,  block->relayed_by,
                                                        block->height, 
                                                        block->timestamp,
                                                        block->hash,
                                                        block->prev_hash,
                                                        block->difficulty,
                                                        block->nonce);
                return;
            }
        }
    }

    printf(SERVER_WRONG_BLOCK, block->height, block->relayed_by, block->hash, hash);
}