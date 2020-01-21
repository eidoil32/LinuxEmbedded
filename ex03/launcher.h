#ifndef MTACOIN_PROGRAM_H
#define MTACOIN_PROGRAM_H 
#include "miscellaneous.h"

void program_start(int num_of_miners);
int loadUpMiners(int num_of_miners);
int loadUpServer();

#define DEFAULT_NUMBER_OF_MINERS        4

#endif