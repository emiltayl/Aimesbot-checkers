#ifndef BOARD_HEADER_H_
#define BOARD_HEADER_H_
#include <stdint.h>

#define POPULATION_COUNT(x) __builtin_popcount(x)
#define GET_BIT(number, bit) number & (1 << bit)
#define SET_BIT(number, bit) number | (1 << bit)
#define UNSET_BIT(number, bit) number & ~(1 << bit)
#define POSITION_IS_WHITE(board, x) GET_BIT(board.white, x)
#define POSITION_IS_BLACK(board, x) GET_BIT(board.black, x)
#define POSITION_IS_KING(board, x) GET_BIT(board.kings, x)
#define POSITION_IS_TAKEN(board, x) POSITION_IS_WHITE(board, x) || POSITION_IS_BLACK(board, x)

typedef uint_fast32_t board;

typedef struct board_struct {
    board black;
    board white;
    board kings;
    board occupied;
} board_t;

board_t input2board(char *);
#endif
