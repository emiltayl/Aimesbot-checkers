#ifndef MOVES_H_
#define MOVES_H_
#include <stdint.h>

#include "checkerboard.h"

#define EVEN 0
#define ODD 1

#define SOUTH_EAST 0
#define SOUTH_WEST 1
#define NORTH_EAST 2
#define NORTH_WEST 3

typedef struct move_struct {
    _board from;
    _board to;
    _board steps[12];
    int nsteps;
} move_t;

typedef struct movelist_struct {
    move_t moves[36];
    int moveCount;
} movelist_t;

int can_even_position_do_jump(_board position, _board *other, int jumpDirection);
int can_odd_position_do_jump(_board position, _board *other, int jumpDirection);
void toggle_jump(_board from, _board to, _board *self, _board *other);
void discover_even_jumps(movelist_t *movelist, move_t *jumpsSoFar, _board *self, _board *other, int jumpNorth, int jumpSouth);
void discover_odd_jumps(movelist_t *movelist, move_t *jumpsSoFar, _board *self, _board *other, int jumpNorth, int jumpSouth);
movelist_t get_self_jumps();
movelist_t get_other_jumps();
/*movelist_t get_possible_moves_white(board_t);
movelist_t get_possible_moves_black(board_t);
do_move_white(&board_t, move_t);
do_move_black(&board_t, move_t);
undo_move_white(&board_t, move_t);
undo_move_black(&board_t, move_t);*/
#endif
