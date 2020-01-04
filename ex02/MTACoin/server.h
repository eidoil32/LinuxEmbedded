#ifndef MTACOIN_SERVER_H
#define MTACOIN_SERVER_H 
#include "miscellaneous.h"

typedef struct server {
    pthread_t server_thread;
    List blocks;
} Server;

void approve_block(Server *server);
void* server_engine(void *server_input);

#endif