#include "server.h"
#include "program.h"

void* server_engine(void *server_input) {
    Server *server = (Server*)server_input;

    while (true) {
        pthread_mutex_lock(&global_block_to_add_lock);
        if (!global_block_to_add) {
            pthread_cond_wait(&global_block_event, &global_block_to_add_lock);
        }

        if (global_block_to_add)
            approve_block(server);
        pthread_mutex_unlock(&global_block_to_add_lock);
        pthread_cond_broadcast(&global_new_block_was_added);
    }
}

void approve_block(Server* server) {
    BLOCK_T prev_block_hash = server->blocks.tail->block;
    PARTIAL_BLOCK_T partial_block = (PARTIAL_BLOCK_T){  global_block_to_add->height, 
                                                    global_block_to_add->timestamp, 
                                                    global_block_to_add->nonce, 
                                                    global_block_to_add->relayed_by, 
                                                    global_block_to_add->prev_hash };
    unsigned int hash = crc32(0L, (Bytef*)&partial_block, sizeof(PARTIAL_BLOCK_T));

    if (global_block_to_add->prev_hash == prev_block_hash.hash) {
        if (global_block_to_add->height == (prev_block_hash.height + 1)) {
            if (hash == global_block_to_add->hash) {
                // locking 'global_last_block_lock' to add an new block to list (so other miners will mine new block with the correct parameters)
                pthread_mutex_lock(&global_last_block_lock);
                addNode(&server->blocks, *global_block_to_add);
                global_last_block = &server->blocks.tail->block;
                pthread_mutex_unlock(&global_last_block_lock);
                printf(SERVER_APPROVED_BLOCK_BY_MINER,  global_block_to_add->relayed_by,
                                                        global_block_to_add->height, 
                                                        global_block_to_add->timestamp,
                                                        global_block_to_add->hash,
                                                        global_block_to_add->prev_hash,
                                                        global_block_to_add->difficulty,
                                                        global_block_to_add->nonce);
                return;
            }
        }
    }

    printf(SERVER_WRONG_BLOCK, global_block_to_add->height, global_block_to_add->relayed_by, global_block_to_add->hash, hash);
    global_block_to_add = NULL;
}