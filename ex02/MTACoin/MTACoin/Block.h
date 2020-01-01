#ifndef MTACOIN_BLOCK_H
#define	MTACOIN_BLOCK_H
#include "Miscellaneous.h"

struct block_t {
    int             height;             // Incrementeal ID of the block in the chain
    int             timestamp;          // Time of the mine in seconds since epoch
    unsigned int    hash;               // Current block hash value
    unsigned int    prev_hash;          // Hash value of the previous block
    int             difficulty;         // Amount of preceding zeros in the hash
    int             nonce;              // Incremental integer to change the hash value
    int             relayed_by;         // Miner ID
};

typedef struct PartialBlock {
    int height, timestamp, nonce, relayed_by;
    unsigned int prev_hash;
} PartialBlock;

// for easier readability
typedef struct block_t**        StaticBlock;
typedef struct block_t*         BLOCK_T;
#define BLOCK_T_SIZE            sizeof(struct block_t)

// block CONST information
#define BLOCK_INIT_DIFFICULY    16

// functions
BLOCK_T initBlock(int id);
PartialBlock initPartialBlock(BLOCK_T block);
unsigned int calcHash(BLOCK_T block);

#endif