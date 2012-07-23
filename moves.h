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

typedef struct jump_struct {
    _board from;
    _board to;
    _board steps[12];
    int nsteps;
} jump_t;

typedef struct jumplist_struct {
    jump_t moves[36];
    int moveCount;
} jumplist_t;

typedef struct movelist_struct {
    _board from[36];
    _board to[36];
    int moveCount;
} movelist_t;

int can_even_position_do_jump(_board position, _board *other, int jumpDirection);
int can_odd_position_do_jump(_board position, _board *other, int jumpDirection);
void toggle_jump(_board from, _board to, _board *self, _board *other);
void discover_even_jumps(jumplist_t *movelist, jump_t *jumpsSoFar, _board *self, _board *other, int jumpNorth, int jumpSouth);
void discover_odd_jumps(jumplist_t *movelist, jump_t *jumpsSoFar, _board *self, _board *other, int jumpNorth, int jumpSouth);
jumplist_t get_self_jumps();
jumplist_t get_other_jumps();
movelist_t get_self_moves();
movelist_t get_other_moves();
void do_jumps(jumplist_t jump, _board *self, _board *other);
void do_move(movelist_t move, _board *self, _board *other);
void undo_jumps(jumplist_t jump, _board *self, _board *other);
void undo_move(movelist_t move, _board *self, _board *other);
#endif
