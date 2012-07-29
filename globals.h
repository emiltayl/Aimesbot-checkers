#ifndef GLOABLS_H_
#define GLOBALS_H_

#include <pthread.h>

#include "checkerboard.h"

#ifdef __GNUC__
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

extern board_t gamestate;
extern unsigned long long nodesVisited;
pthread_mutex_t mutex;

#endif
