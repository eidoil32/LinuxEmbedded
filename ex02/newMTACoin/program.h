#ifndef MTACOIN_PROGRAM_H
#define MTACOIN_PROGRAM_H 
#include "miscellaneous.h"
#include "server.h"
#include "miner.h"

BLOCK_T	        *global_last_block,                 // the last block in block list
                *global_block_to_add;               // the last mined block from miners

pthread_mutex_t global_last_block_lock,             // the lock for 'global_last_block' variable
				global_block_to_add_lock;           // the lock for 'global_block_to_add' variable

pthread_cond_t 	global_block_event,                 // to let server know that miner mined an new block
                global_block_is_available,          // to let miners know that the variable 'global_block_to_add' is now NULL
				global_new_block_was_added;         // to let miners know that new block is added and they should mine new block with other parameters

typedef struct program {
    Server server;
    Miner* miners;
} Program;

void program_start();
void loadUpMiners(Miner** miners);
void loadUpServer(Server* server);

#endif