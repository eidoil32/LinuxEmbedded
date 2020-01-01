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

struct node {
	void* data;
	struct node* next, *prev;
};

typedef struct node* Node;
#define NODE_SIZE sizeof(struct node)

struct list {
	Node head, tail;
};

struct package {
	void **parms;
};

// for easier readability
typedef char* 							String;
typedef struct list* 					List;
typedef struct package* 				Package;
#define LIST_SIZE 						sizeof(struct list)
#define PACKAGE_SIZE					sizeof(struct package)

// functions
void freeList(List list);
Node addNode(List list, void* data);
int getCurrentTimeStamp();

// errors strings
#define ERROR_CREATING_SERVER_OBJECT	"Error: creating server object failed!"
#define ERROR_CREATING_NEW_NODE			"Error: creating new node failed!"
#define ERROR_CREATING_NEW_MINER		"Error: creating new miner failed!"

// strings
#define SERVER_APPROVED_BLOCK_BY_MINER	"Server: New Block added by Miner %d, attributes: height(%d),"\
										" timestamp(%d), hash(0x%02x), prev_hash(0x%02x)," \
										" difficulty(%d), nonce(%d)\n"
#define SERVER_WRONG_BLOCK				"Wrong hash for block #%d by miner %d,"\
										" received 0x%02x but calculated 0x%02x"
										
// MASKS:
#define MASK_LAST_16_BITS				0xFFFF0000 // do with Bitwise AND!

// Readability indexes
#define SERVER_INDEX 					0
#define MINER_INDEX						0
#define LAST_BLOCK_INDEX				1
#define BLOCK_TO_ADD_INDEX  			2
#define NEW_BLOCK_BLOCK_INDEX 			0
#define NEW_BLOCK_MINER_INDEX			1

// debug print
#define DEBUG(str) 						printf(str)
#endif