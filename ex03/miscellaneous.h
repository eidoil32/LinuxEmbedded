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
#include <mqueue.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

typedef struct node {
    BLOCK_T block;
    struct node *next, *prev;
} Node;

typedef struct list {
    Node *head, *tail;
    size_t size;
} List;

#define SERVER_MESSAGE_QUEUE            "/server_mq"
#define MINER_MESSAGE_QUEUE_PREFIX      "/miner_mq_"
#define MINER_EXECVE_PROGRAM_NAME       "./miner"
#define SERVER_EXECVE_PROGRAM_NAME      "./server"
#define MAX_SUPPORTED_NUM_OF_DIGITS     3   // maximum supported miners is 999

// strings
#define SERVER_APPROVED_BLOCK_BY_MINER	    "Server: New Block added by Miner %d, attributes: height(%d),"\
										    " timestamp(%d), hash(0x%02x), prev_hash(0x%02x)," \
										    " difficulty(%d), nonce(%d)\n"
#define MINER_MINED_NEW_BLOCK			    "Miner #%d: Mined a new block #%d, with the hash 0x%x\n"
#define SERVER_WRONG_BLOCK				    "Wrong hash for block #%d by Miner %d,"\
										    " received 0x%02x but calculated 0x%02x\n"
#define MINER_RECEIVED_BLOCK_PREFIX         " block"
#define MINER_RECEIVED_BLOCK                "Miner %d received %s: relayed_by(%d),"\
										    " height(%d), timestamp(%d), hash(0x%02x), prev_hash(0x%02x)," \
										    " difficulty(%d), nonce(%d)\n"
#define MINER_SENT_CONNECTION_REQUEST       "Miner %d sent connect request on %s\n"
#define MINER_WELCOME_MESSAGE               "Miner id = %d, queue name = %s\n"
#define MINER_RECEIVED_FIRST_BLOCK_PREFIX   " first block"
#define FORK_FAILED                         "fork() failed, %s not created!\n"
#define EXECVE_FAILED                       "execve() failed, %s not created!\n"
#define LOAD_UP_FAILD                       "loading up %s failed!\n"
#define SERVER_STRING                       "server"
#define MINER_STRING                        "miner"
#define SERVER_LISTENING_MSG_QUEUE          "Listening on %s\n"
#define RECEIVED_NEW_MINER_CONNECTION		"Received connection request from miner id %d, queue name %s\n"
#define SEND_BLOCK_TO_MINER_FAILED			"Sending block to miner %d failed!\n"
#define SEND_BLOCK_TO_SERVER_FAILD			"Sending block from miner %d failed!\n"

// MASKS:
#define MASK_LAST_16_BITS				0xFFFF0000 // do with Bitwise AND!

void initList(List *list);
Node* addNode(List *list, BLOCK_T block);
int getCurrentTimeStamp();
char *itoa(int number);

#endif