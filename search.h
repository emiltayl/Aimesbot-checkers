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
heuristic_t alphaSearch(int depth, heuristic_t alpha, heuristic_t beta, unsigned int *);
heuristic_t betaSearch(int depth, heuristic_t alpha, heuristic_t beta, unsigned int *);

#endif
