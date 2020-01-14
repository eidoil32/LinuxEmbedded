#ifndef MTACOIN_BLOCK_H
#define MTACOIN_BLOCK_H 

typedef struct BLOCK_T {
    int         height;             // Incrementeal ID of the block in the chain
    int         timestamp;          // Time of the mine in seconds since epoch
    unsigned int     hash;          // Current block hash value
    unsigned int     prev_hash;     // Hash value of the previous block
    int        difficulty;          // Amount of preceding zeros in the hash
    int         nonce;              // Incremental integer to change the hash value
    int         relayed_by;         // Miner ID
} BLOCK_T;

typedef struct PARTIAL_BLOCK_T {
    int height, timestamp, nonce, relayed_by;
    unsigned int prev_hash;
} PARTIAL_BLOCK_T;

// block CONST information
#define BLOCK_INIT_DIFFICULY        16
#define INITIALIZE_BLOCK_T          {0, 0, 0, 0, BLOCK_INIT_DIFFICULY, 0, 0}

BLOCK_T initBlock(int miner_id, int height, unsigned int prev_hash);
PARTIAL_BLOCK_T create_partial_block(BLOCK_T *block);

#define EMPTY_BLOCK                 (BLOCK_T){ 0 }

#endif