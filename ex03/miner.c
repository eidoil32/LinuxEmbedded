#include "miner.h"

Miner miner; // kind of singleton // no it's not, singleton sucks and belong in crappy C#
mqd_t miner_mq, server_mq;

int main(int argc, char* argv[]) {
    miner.miner_id = atoi(argv[1]);
    miner_engine(); // server engine start..
    return 0;
}

void miner_engine() {
    MQ_Message *message;
    BLOCK_T current_last_block = EMPTY_BLOCK, self_new_block;
    bool first_block = true;

    //creating message queue name "/miner_mq_X" and X is the ID of miner
    char *miner_mq_name = (char*)calloc(strlen(MINER_MESSAGE_QUEUE_PREFIX) + MAX_SUPPORTED_NUM_OF_DIGITS + 1, sizeof(char));
    sprintf(miner_mq_name, "%s%d", MINER_MESSAGE_QUEUE_PREFIX, miner.miner_id);
    printf(MINER_WELCOME_MESSAGE, miner.miner_id, miner_mq_name);

    struct mq_attr miner_attr = { 0 };
    miner_attr.mq_maxmsg = MQ_MINER_MAX_MESSAGES;
    miner_attr.mq_msgsize = MQ_MINER_MAX_MSG_SIZE;

    mq_unlink(miner_mq_name);  // free old mq from memory
    miner_mq = mq_open(miner_mq_name, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG, &miner_attr);
    server_mq = mq_open(SERVER_MESSAGE_QUEUE, O_RDWR);

    message = (MQ_Message*)malloc(sizeof(Miner) + sizeof(MQ_Message));
    message->type = E_MINER;
    *((Miner*)message->data) = miner;
    if (mq_send(server_mq, (char*)message, MQ_SERVER_MAX_MSG_SIZE, 0) == 0) {
        printf(MINER_SENT_CONNECTION_REQUEST, miner.miner_id, miner_mq_name);
    }

    while(true) {
        BLOCK_T newes_block = current_last_block;
        mq_receive(miner_mq, (char*)&current_last_block, MQ_MINER_MAX_MSG_SIZE, NULL); // miner will wait until server will send it the last block
        if (newes_block.height <= current_last_block.height) {
            printf(MINER_RECEIVED_BLOCK,    miner.miner_id,
                                            (first_block ? MINER_RECEIVED_FIRST_BLOCK_PREFIX : MINER_RECEIVED_BLOCK_PREFIX), 
                                            current_last_block.relayed_by,
                                            current_last_block.height,
                                            current_last_block.timestamp,
                                            current_last_block.hash,
                                            current_last_block.prev_hash,
                                            current_last_block.difficulty,
                                            current_last_block.nonce);
        } else {
            current_last_block = newes_block;
        }
        first_block = false;    


        if (current_last_block.hash != 0) {
            self_new_block = calculate_block(miner.miner_id, current_last_block);

            // sending new block to server
            message = (MQ_Message*)malloc(sizeof(MQ_Message) + sizeof(BLOCK_T));
            message->type = E_BLOCK;
            *((BLOCK_T*)message->data) = self_new_block;
            if (mq_send(server_mq, (char*)message, MQ_SERVER_MAX_MSG_SIZE, 0) == 0)
                printf(MINER_MINED_NEW_BLOCK, miner.miner_id, self_new_block.height, self_new_block.hash);
            else {
                fprintf(stderr, SEND_BLOCK_TO_SERVER_FAILD, miner.miner_id);
            }
        }
    }
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
