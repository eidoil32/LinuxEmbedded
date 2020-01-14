#include "miner.h"

Miner miner; // kind of singleton
mqd_t miner_mq, server_mq;

int main(int argc, char* argv[]) {
    miner.miner_id = atoi(argv[1]);
    miner_engine(); // server engine start..
    return 0;
}

void open_mq(char *mq_name, struct mq_attr* attr, int max_mq_size, int max_msg_size) { 
    attr->mq_maxmsg  = max_mq_size;
    attr->mq_msgsize = max_msg_size;
    miner_mq = mq_open(mq_name, O_CREAT, S_IRWXU | S_IRWXG, attr);
}

void miner_engine() {
    bool first_block = true;
    BLOCK_T current_last_block, self_new_block, temp_block;

    //creating message queue name "/miner_mq_X" and X is the ID of miner
    char *miner_mq_name = (char*)calloc(strlen(MINER_MESSAGE_QUEUE_PREFIX) + MAX_SUPPORTED_NUM_OF_DIGITS + 1, sizeof(char));
    sprintf(miner_mq_name, "%s%d", MINER_MESSAGE_QUEUE_PREFIX, miner.miner_id);
    printf(MINER_WELCOME_MESSAGE, miner.miner_id, miner_mq_name);

    struct mq_attr server_attr = { 0 }, miner_attr = { 0 };

    mq_unlink(miner_mq_name);  // free old mq from memory
    open_mq(miner_mq_name, &miner_attr, MQ_MINER_MAX_MESSAGES, MQ_MINER_MAX_MSG_SIZE);

    open_mq(SERVER_MESSAGE_QUEUE, &server_attr, MQ_SERVER_MAX_MESSAGES, MQ_SERVER_MAX_MSG_SIZE);
    printf(MINER_SENT_CONNECTION_REQUEST, miner.miner_id, miner_mq_name);
    mq_send(server_mq, (char*)&miner, MQ_SERVER_MAX_MSG_SIZE, NULL);

    while(true) {
        mq_receive(miner_mq, (char*)&temp_block, MQ_MINER_MAX_MSG_SIZE, NULL); // miner will wait until server will send it the last block
        printf(MINER_RECEIVED_BLOCK,    first_block ? MINER_RECEIVED_FIRST_BLOCK_PREFIX : MINER_RECEIVED_BLOCK_PREFIX, 
                                        miner.miner_id, 
                                        current_last_block.relayed_by,
                                        current_last_block.height,
                                        current_last_block.timestamp,
                                        current_last_block.hash,
                                        current_last_block.prev_hash,
                                        current_last_block.difficulty,
                                        current_last_block.nonce);
        first_block = false;    
        self_new_block = calculate_block(miner.miner_id, current_last_block);

        mq_getattr(miner_mq, &miner_attr); 
        if (miner_attr.mq_curmsgs == MQ_MINER_MAX_MESSAGES) continue; // the server may send an new block in the queue

        // sending new block to server
        mq_send(miner_mq, (char*)&self_new_block, MQ_MINER_MAX_MSG_SIZE, NULL);
        printf(MINER_MINED_NEW_BLOCK, miner.miner_id, self_new_block.height, self_new_block.hash);

        do { // miner will wait until server takes the new block
            mq_getattr(miner_mq, &miner_attr); 
        } while (miner_attr.mq_curmsgs == MQ_MINER_MAX_MESSAGES);
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
    PARTIAL_BLOCK_T partial_block = create_partial_block(&block);
    block.hash = crc32(0, (Bytef*)&partial_block, sizeof(PARTIAL_BLOCK_T));

    while ((block.hash & MASK_LAST_16_BITS) != 0) { //checking if the hash we got is with 16 zeros at the beginning
        partial_block.nonce = block.nonce = block.nonce + 1;
        block.hash = crc32(0, (Bytef*)&partial_block, sizeof(PARTIAL_BLOCK_T));
    }

    return block;
}