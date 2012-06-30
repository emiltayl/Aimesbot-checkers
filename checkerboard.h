#ifndef BOARD_HEADER_H_
#define BOARD_HEADER_H_
#include <stdint.h>

#define GET_BIT(number, bit) (number & (1 << bit))
#define SET_BIT(number, bit) (number | (1 << bit))
#define UNSET_BIT(number, bit) (number & ~(1 << bit))
#define POSITION_IS_WHITE(board, x) GET_BIT(board.white, x)
#define POSITION_IS_BLACK(board, x) GET_BIT(board.black, x)
#define POSITION_IS_KING(board, x) GET_BIT(board.kings, x)
#define POSITION_IS_TAKEN(board, x) POSITION_IS_WHITE(board, x) || POSITION_IS_BLACK(board, x)

typedef uint_fast32_t _board;

typedef struct board_struct {
    _board black;
    _board white;
    _board kings;
    _board occupied;
} board_t;

board_t input2board(char *);
unsigned int population_count(_board);
#endif
