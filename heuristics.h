#ifndef HEURISTIC_HEADER_H_
#define HEURISTIC_HEADER_H_
#include <stdint.h>

#include "checkerboard.h"

#define HEURISTIC_MAX INT_FAST32_MAX
#define HEURISTIC_MIN INT_FAST32_MIN
#define HEURISTIC_WIN 65535
#define HEURISTIC_LOSS -65535

typedef int_fast32_t heuristic_t;

heuristic_t calculate_heuristics(int isItSelfTurn);

#endif
