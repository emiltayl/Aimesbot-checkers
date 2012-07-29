#ifndef SEARCH_H_
#define SEARCH_H_

#include "checkerboard.h"
#include "heuristics.h"
#include "moves.h"

extern jump_t *global_bestJump;
extern _board *global_bestFrom;
extern _board *global_bestTo;
extern int depthSearched;

void *runSearch(void *ptr);
heuristic_t alphaSearch(int depth, int alpha, int beta);
heuristic_t betaSearch(int depth, int alpha, int beta);

#endif
