#ifndef HEURISTIC_HEADER_H_
#define HEURISTIC_HEADER_H_
#include <stdint.h>

#include "checkerboard.h"

#define HEURISTIC_MAX INT32_MAX
#define HEURISTIC_MIN INT32_MIN
#define HEURISTIC_WIN 65535
#define HEURISTIC_LOSS -65535

typedef int32_t heuristic_t;

heuristic_t calculate_heuristics(int isItSelfTurn);

#endif
