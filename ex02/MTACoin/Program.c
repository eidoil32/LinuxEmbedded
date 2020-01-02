#include "Program.h"

int main(int argc, String argv[]) {
	start(); // Program.Start();
	return 0;
}

void start() {
	Program program;
	global_lastBlock 			= (struct block_t**)calloc(1, sizeof(struct block_t*));
	global_blockToAdd			= (struct block_t**)calloc(1, sizeof(struct block_t*));
	global_lastBlock_lock 		= PTHREAD_MUTEX_INITIALIZER, 
	global_blockToAdd_lock 		= PTHREAD_MUTEX_INITIALIZER;

	global_blockEvent 			= PTHREAD_COND_INITIALIZER,
	global_newBlockWasAdded 	= PTHREAD_COND_INITIALIZER;
	program.server = initServer();
	program.miners = (Miners)malloc(MINERS_SIZE);

	// initialization first block for server
	addNode(program.server->blocks, createFirstBlock());
	global_lastBlock = (struct block_t**) &program.server->blocks->head->data;
	
	loadUpServer(program.server);
	loadUpMiners(program.miners);

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
	pthread_create(&server->threadID, NULL, serverEngine, server);
}

void loadUpMiners(Miners miners) {
	for (int i = 0; i < NUM_OF_MINERS; ++i) {
		Miner miner = initMiner(i + 1);
		if (miner) { // checking for memory allocate
			if (!addNode(miners, miner)) {
				fprintf(stderr, ERROR_CREATING_NEW_NODE);
			} else { // adding miner to list successeded, not create a thread for him
				pthread_create(&miner->threadID, NULL, minerEngine, miner);
			}
		} else {
			fprintf(stderr, ERROR_CREATING_NEW_MINER); // creating miner struct failed
		}
	}
}

void programEnd(Program program) {
	freeList(program.miners); 			// deleting all miners
	free(program.miners);
	freeList(program.server->blocks); 	// deleting all blocks from memory
	free(program.server->blocks); 
	free(program.server); 
}