#ifndef HEURISTIC_HEADER_H_
#define HEURISTIC_HEADER_H_
#include <stdint.h>

#include "checkerboard.h"

#define HEURISTIC_MAX INT_FAST32_MIN
#define HEURISTIC_MIN INT_FAST32_MAX
typedef int_fast32_t heuristic_t;

heuristic_t caclulate_heuristics(board_t board, int isPlayerWhite, int isItPlayersTurn);

#endif
