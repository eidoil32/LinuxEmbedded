#ifndef MTACOIN_PROGRAM_H
#define MTACOIN_PROGRAM_H
#include "Miscellaneous.h"
#include "Server.h"
#include "Block.h"
#include "Miner.h"


typedef struct program {
	Server server;
	Miners miners;
}Program;

void start();
Package initPackage(void **parms);
void programEnd(Program program);
void loadUpServer(Server server);
void loadUpMiners(Miners miners);
void waitForThreads(Program program);

#define NUM_OF_MINERS 			4
#define MINERS_SIZE 			LIST_SIZE

#endif // !MTACOIN_PROGRAM_H