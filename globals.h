#ifndef GLOABLS_H_
#define GLOBALS_H_

#include <pthread.h>

#include "checkerboard.h"

extern board_t gamestate;
extern unsigned long long nodesVisited;
pthread_mutex_t mutex;

#endif
