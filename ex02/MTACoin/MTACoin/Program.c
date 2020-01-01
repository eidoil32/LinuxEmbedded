#include "Program.h"
#include "Server.h"
#include "Block.h"
#include "Miner.h"

/* 	if NULL then no blocks in system, miners will need to wait for server to create the first block,
	if *NOT* NULL then miner will take this block to calculate the next block.
	it will be pass as arguments to the function beacuse miner, server and program
	is in differents files. */
static StaticBlock lastBlock;
/*	if NULL then server will wait for miners to create the next block,
	if *NOT* NULL then server will need check if the block is ok and add it to list.
	When server taking block to check he will change 'blockToAdd' to NULL again. */
static Package blockToAdd = NULL;

pthread_mutex_t lastBlock_lock 	= PTHREAD_MUTEX_INITIALIZER, 
				blockToAdd_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t 	blockEvent = PTHREAD_COND_INITIALIZER,
				newBlockWasAdded = PTHREAD_COND_INITIALIZER;

int main(int argc, String argv[]) {
	start(); // Program.Start();
	return 0;
}

void start() {
	Program program;
	program.server = initServer();
	program.miners = (Miners)malloc(MINERS_SIZE);

	loadUpMiners(program.miners);
	loadUpServer(program.server);

	pthread_join(program.server->threadID, NULL); // wait for server to finish == NEVER! user need to stop application; 
												  // can be replace with: waitForThreads(program) ?
												  // it's enough waiting for server?
	programEnd(program);
}

void waitForThreads(Program program) {
	pthread_join(program.server->threadID, NULL); // wait for server;
	
	Node miner = program.miners->head;
	while (miner) {
		pthread_join(((Miner)miner->data)->threadID, NULL);
	}
}

void loadUpServer(Server server) {
	void **parms = (void**)malloc(3 * sizeof(void*));
	parms[SERVER_INDEX] 		= server;
	parms[LAST_BLOCK_INDEX] 	= &lastBlock;
	parms[BLOCK_TO_ADD_INDEX] 	= &blockToAdd;
	Package pack = initPackage(parms);
	server->blockToAdd_lock = blockToAdd_lock;
	server->lastBlock_lock = lastBlock_lock;
	server->newBlockWasAdded = newBlockWasAdded;
	server->blockEvent = blockEvent;
	pthread_create(&server->threadID, NULL, serverEngine, pack);
}

void loadUpMiners(Miners miners) {
	for (int i = 0; i < NUM_OF_MINERS; ++i) {
		Miner miner = initMiner(i + 1);
		if (miner) { // checking for memory allocate
			if (!addNode(miners, miner)) {
				fprintf(stderr, ERROR_CREATING_NEW_NODE);
			} else { // adding miner to list successeded, not create a thread for him
				miner->blockToAdd_lock = blockToAdd_lock;
				miner->lastBlock_lock = lastBlock_lock;
				miner->newBlockWasAdded = newBlockWasAdded;
				miner->blockEvent = blockEvent;
				void **parms = (void**)malloc(3 * sizeof(void*));
				parms[MINER_INDEX] 			= miner;
				parms[LAST_BLOCK_INDEX] 	= &lastBlock;
				parms[BLOCK_TO_ADD_INDEX] 	= &blockToAdd;
				Package pack = initPackage(parms);
				pthread_create(&miner->threadID, NULL, minerEngine, pack);
			}
		} else {
			fprintf(stderr, ERROR_CREATING_NEW_MINER); // creating miner struct failed
		}
	}
}

Package initPackage(void **parms) {
	Package package = (Package)malloc(PACKAGE_SIZE);
	package->parms = parms;
	return package;
}

void programEnd(Program program) {
	freeList(program.miners); 			// deleting all miners
	free(program.miners);
	freeList(program.server->blocks); 	// deleting all blocks from memory
	free(program.server->blocks); 
	free(program.server); 
}