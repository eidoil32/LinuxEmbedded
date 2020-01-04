#include "program.h"

int main(int argc, char* argv[]) {
    program_start();
    return 0;
}

void program_start() {
    // initialize global locks and events conditions varibales
    global_last_block_lock          = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    global_block_to_add_lock        = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    global_block_event              = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    global_block_is_available       = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    global_new_block_was_added      = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

    Program program;
    initList(&(program.server.blocks));

    // creating first block
    BLOCK_T block = initBlock(0, -2, 0);
    block = calculate_block(0, block);
    addNode(&program.server.blocks, block);
    global_last_block = &block;

    loadUpServer(&program.server);
    loadUpMiners(&program.miners);
    pthread_join(program.server.server_thread, NULL);
}

void loadUpMiners(Miner** miners) {
    *miners = (Miner*)calloc(MINER_ARRAY_SIZE, sizeof(Miner));

    for (size_t i = 1; i <= MINER_ARRAY_SIZE; ++i) {
        (*miners)[i - 1].system_id = i;
        if (i == MINER_ARRAY_SIZE) {
            (*miners)[i - 1].create_block = fake_miner_block_creator;
        } else {
            (*miners)[i - 1].create_block = calculate_block;
        }
        pthread_create(&(*miners)[i - 1].id, NULL, miner_engine, &(*miners)[i - 1]);
    }
}

void loadUpServer(Server* server) {
    pthread_create(&server->server_thread, NULL, server_engine, server);
}