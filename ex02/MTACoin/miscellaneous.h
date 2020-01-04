#ifndef MTACOIN_MISCELLANEOUS_H
#define	MTACOIN_MISCELLANEOUS_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <zlib.h>
#include <time.h>
#include "block.h"

typedef struct node {
    BLOCK_T block;
    struct node *next, *prev;
} Node;

typedef struct list {
    Node *head, *tail;
    size_t size;
} List;

#define MINER_ARRAY_SIZE                5

// strings
#define SERVER_APPROVED_BLOCK_BY_MINER	"Server: New Block added by Miner %d, attributes: height(%d),"\
										" timestamp(%d), hash(0x%02x), prev_hash(0x%02x)," \
										" difficulty(%d), nonce(%d)\n"
#define MINER_MINED_NEW_BLOCK			"Miner #%d: Mined a new block #%d, with the hash 0x%x\n"
#define SERVER_WRONG_BLOCK				"Wrong hash for block #%d by Miner %d,"\
										" received 0x%02x but calculated 0x%02x\n"

// MASKS:
#define MASK_LAST_16_BITS				0xFFFF0000 // do with Bitwise AND!

void initList(List *list);
Node* addNode(List *list, BLOCK_T block);
int getCurrentTimeStamp();

#endif