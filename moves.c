#include "moves.h"
#include "globals.h"
#include "checkerboard.h"

static _board jumpMasks[2][4] = {
    { //even
        0x00707070, //South-East
        0x00E0E0E0, //South-West
        0x70707000, //North-East
        0xE0E0E000  //North-West
    },
    { //odd
        0x00070707, //South-East
        0x000E0E0E, //South-West
        0x07070700, //North-East
        0x0E0E0E00  //North-West
    }
};

static _board jumpModifiers[2][4][2] = {
    { //even
        { //South-East
            4,
            9
        },
        { //South-West
            3,
            7
        },
        { //North-East
            4,
            7
        },
        { //North-West
            5,
            9
        }
    },
    { //odd
        { //South-East
            5,
            9
        },
        { //South-West
            4,
            7
        },
        { //North-East
            3,
            7
        },
        { //North-West
            4,
            9
        }
    }
};

int can_even_position_do_jump(_board position, _board *other, int jumpDirection) {
    if (jumpDirection & 2) {
        //North
        return (position & jumpMasks[EVEN][jumpDirection])
            && (*other & (position >> jumpModifiers[EVEN][jumpDirection][0]))
            && !(gamestate.occupied & (position >> jumpModifiers[EVEN][jumpDirection][1]));
    } else {
        //South
        return (position & jumpMasks[EVEN][jumpDirection])
            && (*other & (position << jumpModifiers[EVEN][jumpDirection][0]))
            && !(gamestate.occupied & (position << jumpModifiers[EVEN][jumpDirection][1]));
    }
}

int can_odd_position_do_jump(_board position, _board *other, int jumpDirection) {
    if (jumpDirection & 2) {
        //North
        return (position & jumpMasks[ODD][jumpDirection])
            && (*other & (position >> jumpModifiers[ODD][jumpDirection][0]))
            && !(gamestate.occupied & (position >> jumpModifiers[ODD][jumpDirection][1]));
    } else {
        //South
        return (position & jumpMasks[ODD][jumpDirection])
            && (*other & (position << jumpModifiers[ODD][jumpDirection][0]))
            && !(gamestate.occupied & (position << jumpModifiers[ODD][jumpDirection][1]));
    }
}

void toggle_jump(_board from, _board to, _board *self, _board *other) {
    int odd = !!(from & 0x0F0F0F0F);
    int west;

    if (from > to) {
        //North
        west = (from >> 7) > to;

        *other = *other ^ (from >> jumpModifiers[odd][2 | west][0]);
        *self = *self ^ (from | to);
        gamestate.occupied = *self | *other;
    } else {
        //South
        west = (from << 7) == to;

        *other = *other ^ (from << jumpModifiers[odd][west][0]);
        *self = *self ^ (from | to);
        gamestate.occupied = *self | *other;
    }
}

void discover_even_jumps(jumplist_t *movelist, jump_t *jumpsSoFar, _board *self, _board *other, int jumpNorth, int jumpSouth) {
    int jumpDone = 0;
    _board pos = jumpsSoFar->to;

    jumpsSoFar->nsteps++;

    if (jumpNorth) {
        if (can_even_position_do_jump(pos, other, NORTH_WEST)) {
            jumpDone = 1;

            jumpsSoFar->steps[jumpsSoFar->nsteps] = pos >> jumpModifiers[EVEN][NORTH_WEST][1];
            jumpsSoFar->to = jumpsSoFar->steps[jumpsSoFar->nsteps];
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
            discover_even_jumps(movelist, jumpsSoFar, self, other, jumpNorth, jumpSouth);
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
        }

        if (can_even_position_do_jump(pos, other, NORTH_EAST)) {
            jumpDone = 1;

            jumpsSoFar->steps[jumpsSoFar->nsteps] = pos >> jumpModifiers[EVEN][NORTH_EAST][1];
            jumpsSoFar->to = jumpsSoFar->steps[jumpsSoFar->nsteps];
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
            discover_even_jumps(movelist, jumpsSoFar, self, other, jumpNorth, jumpSouth);
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
        }
    }

    if (jumpSouth) {
        if (can_even_position_do_jump(pos, other, SOUTH_WEST)) {
            jumpDone = 1;

            jumpsSoFar->steps[jumpsSoFar->nsteps] = pos << jumpModifiers[EVEN][SOUTH_WEST][1];
            jumpsSoFar->to = jumpsSoFar->steps[jumpsSoFar->nsteps];
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
            discover_even_jumps(movelist, jumpsSoFar, self, other, jumpNorth, jumpSouth);
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
        }

        if (can_even_position_do_jump(pos, other, SOUTH_EAST)) {
            jumpDone = 1;

            jumpsSoFar->steps[jumpsSoFar->nsteps] = pos << jumpModifiers[EVEN][SOUTH_EAST][1];
            jumpsSoFar->to = jumpsSoFar->steps[jumpsSoFar->nsteps];
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
            discover_even_jumps(movelist, jumpsSoFar, self, other, jumpNorth, jumpSouth);
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
        }
    }

    jumpsSoFar->nsteps--;

    if (!jumpDone) {
        movelist->moves[movelist->moveCount++] = *jumpsSoFar;
    }
}

void discover_odd_jumps(jumplist_t *movelist, jump_t *jumpsSoFar, _board *self, _board *other, int jumpNorth, int jumpSouth) {
    int jumpDone = 0;
    _board pos = jumpsSoFar->to;

    jumpsSoFar->nsteps++;

    if (jumpNorth) {
        if (can_odd_position_do_jump(pos, other, NORTH_WEST)) {
            jumpDone = 1;

            jumpsSoFar->steps[jumpsSoFar->nsteps] = pos >> jumpModifiers[ODD][NORTH_WEST][1];
            jumpsSoFar->to = jumpsSoFar->steps[jumpsSoFar->nsteps];
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
            discover_odd_jumps(movelist, jumpsSoFar, self, other, jumpNorth, jumpSouth);
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
        }

        if (can_odd_position_do_jump(pos, other, NORTH_EAST)) {
            jumpDone = 1;

            jumpsSoFar->steps[jumpsSoFar->nsteps] = pos >> jumpModifiers[ODD][NORTH_EAST][1];
            jumpsSoFar->to = jumpsSoFar->steps[jumpsSoFar->nsteps];
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
            discover_odd_jumps(movelist, jumpsSoFar, self, other, jumpNorth, jumpSouth);
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
        }
    }

    if (jumpSouth) {
        if (can_odd_position_do_jump(pos, other, SOUTH_WEST)) {
            jumpDone = 1;

            jumpsSoFar->steps[jumpsSoFar->nsteps] = pos << jumpModifiers[ODD][SOUTH_WEST][1];
            jumpsSoFar->to = jumpsSoFar->steps[jumpsSoFar->nsteps];
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
            discover_odd_jumps(movelist, jumpsSoFar, self, other, jumpNorth, jumpSouth);
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
        }

        if (can_odd_position_do_jump(pos, other, SOUTH_EAST)) {
            jumpDone = 1;

            jumpsSoFar->steps[jumpsSoFar->nsteps] = pos << jumpModifiers[ODD][SOUTH_EAST][1];
            jumpsSoFar->to = jumpsSoFar->steps[jumpsSoFar->nsteps];
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
            discover_odd_jumps(movelist, jumpsSoFar, self, other, jumpNorth, jumpSouth);
            toggle_jump(pos, jumpsSoFar->steps[jumpsSoFar->nsteps], self, other);
        }
    }

    jumpsSoFar->nsteps--;

    if (!jumpDone) {
        movelist->moves[movelist->moveCount++] = *jumpsSoFar;
    }
}

jumplist_t get_self_jumps() {
    _board i = 0x80000000,
           n,
           selfKings = gamestate.self & gamestate.kings;

    jump_t jump;
    jumplist_t jumps;

    jump.nsteps = 0;
    jumps.moveCount = 0;

    while (i > 0x00000080) {
        n = i >> 4;
        for (; i > n; i = i >> 1) {
            //Even rows
            if (i & gamestate.self) {
                if (can_even_position_do_jump(i, &gamestate.other, NORTH_WEST)
                        || can_even_position_do_jump(i, &gamestate.other, NORTH_EAST)
                        || ((i & selfKings)
                            && (can_even_position_do_jump(i, &gamestate.other, SOUTH_WEST)
                                || can_even_position_do_jump(i, &gamestate.other, SOUTH_EAST)))) {
                    jump.from = jump.to = i;
                    jump.steps[0] = i;
                    discover_even_jumps(&jumps, &jump, &gamestate.self, &gamestate.other, 1, i & selfKings);
                }
            }
        }

        n = i >> 4;
        for (; i > n; i = i >> 1) {
            //Odd rows
            if (i & gamestate.self) {
                if (can_odd_position_do_jump(i, &gamestate.other, NORTH_WEST)
                        || can_odd_position_do_jump(i, &gamestate.other, NORTH_EAST)
                        || ((i & selfKings)
                            && (can_odd_position_do_jump(i, &gamestate.other, SOUTH_WEST)
                                || can_odd_position_do_jump(i, &gamestate.other, SOUTH_EAST)))) {
                    jump.from = jump.to = i;
                    jump.steps[0] = i;
                    discover_odd_jumps(&jumps, &jump, &gamestate.self, &gamestate.other, 1, i & selfKings);
                }
            }
        }
    }

    for (; i > 0x00000008; i = i >> 1) {
        if (i & selfKings) {
            if (can_even_position_do_jump(i, &gamestate.other, SOUTH_WEST)
                || can_even_position_do_jump(i, &gamestate.other, SOUTH_EAST)) {
                jump.from = jump.to = i;
                jump.steps[0] = i;
                discover_even_jumps(&jumps, &jump, &gamestate.self, &gamestate.other, 1, 1);
            }
        }
    }

    for (; i; i = i >> 1) {
        if (i & selfKings) {
            if (can_odd_position_do_jump(i, &gamestate.other, SOUTH_WEST)
                || can_odd_position_do_jump(i, &gamestate.other, SOUTH_EAST)) {
                jump.from = jump.to = i;
                jump.steps[0] = i;
                discover_odd_jumps(&jumps, &jump, &gamestate.self, &gamestate.other, 1, 1);
            }
        }
    }

    return jumps;
}

jumplist_t get_other_jumps() {
    _board i = 0x80000000,
           n,
           otherKings = gamestate.other & gamestate.kings;

    jump_t jump;
    jumplist_t jumps;

    jump.nsteps = 0;
    jumps.moveCount = 0;

    for (; i > 0x08000000; i = i >> 1) {
        if (i & otherKings) {
            if (can_even_position_do_jump(i, &gamestate.self, NORTH_WEST)
                || can_even_position_do_jump(i, &gamestate.self, NORTH_EAST)) {
                jump.from = jump.to = i;
                jump.steps[0] = i;
                discover_even_jumps(&jumps, &jump, &gamestate.other, &gamestate.self, 1, 1);
            }
        }
    }

    for (; i > 0x00800000; i = i >> 1) {
        if (i & otherKings) {
            if (can_odd_position_do_jump(i, &gamestate.self, NORTH_WEST)
                || can_odd_position_do_jump(i, &gamestate.self, NORTH_EAST)) {
                jump.from = jump.to = i;
                jump.steps[0] = i;
                discover_odd_jumps(&jumps, &jump, &gamestate.other, &gamestate.self, 1, 1);
            }
        }
    }

    while (i) {
        n = i >> 4;
        for (; i > n; i = i >> 1) {
            //Even rows
            if (i & gamestate.other) {
                if (can_even_position_do_jump(i, &gamestate.self, SOUTH_WEST)
                        || can_even_position_do_jump(i, &gamestate.self, SOUTH_EAST)
                        || ((i & otherKings)
                            && (can_even_position_do_jump(i, &gamestate.self, NORTH_WEST)
                                || can_even_position_do_jump(i, &gamestate.self, NORTH_EAST)))) {
                    jump.from = jump.to = i;
                    jump.steps[0] = i;
                    discover_even_jumps(&jumps, &jump, &gamestate.other, &gamestate.self, i & otherKings, 1);
                }
            }
        }

        n = i >> 4;
        for (; i > n; i = i >> 1) {
            //Odd rows
            if (i & gamestate.other) {
                if (can_odd_position_do_jump(i, &gamestate.self, SOUTH_WEST)
                        || can_odd_position_do_jump(i, &gamestate.self, SOUTH_EAST)
                        || ((i & otherKings)
                            && (can_odd_position_do_jump(i, &gamestate.self, NORTH_WEST)
                                || can_odd_position_do_jump(i, &gamestate.self, NORTH_EAST)))) {
                    jump.from = jump.to = i;
                    jump.steps[0] = i;
                    discover_odd_jumps(&jumps, &jump, &gamestate.other, &gamestate.self, i & otherKings, 1);
                }
            }
        }
    }

    return jumps;
}

movelist_t get_self_moves() {
    int odd;

    movelist_t movelist;

    _board board,
           position,
           selfKings = gamestate.self & gamestate.kings,
           noccupied = ~gamestate.occupied;

    movelist.moveCount = 0;

    //North-east
    board = ((0xF0F0F0F0 & gamestate.self) >> 4) & noccupied;
    board |= ((0x07070700 & gamestate.self) >> 3) & noccupied;

    while (board) {
        position = 0x80000000 >> __builtin_clz(board);
        odd = !(position & 0x0F0F0F0F);

        movelist.to[movelist.moveCount] = position;
        movelist.from[movelist.moveCount++] = position << (4 - odd);

        board = board ^ position;
    }

    //North-west
    board = ((0xE0E0E0E0 & gamestate.self) >> 5) & noccupied;
    board |= ((0x0F0F0F00 & gamestate.self) >> 4) & noccupied;

    while (board) {
        position = 0x80000000 >> __builtin_clz(board);
        odd = !(position & 0x0F0F0F0F);

        movelist.to[movelist.moveCount] = position;
        movelist.from[movelist.moveCount++] = position << (5 - odd);

        board = board ^ position;
    }

    //South-east
    board = ((0x00F0F0F0 & selfKings) << 4) & noccupied;
    board |= ((0x07070707 & selfKings) << 5) & noccupied;

    while (board) {
        position = 0x80000000 >> __builtin_clz(board);
        odd = !(position & 0x0F0F0F0F);

        movelist.to[movelist.moveCount] = position;
        movelist.from[movelist.moveCount++] = position >> (4 + odd);

        board = board ^ position;
    }

    //South-west
    board = ((0x00E0E0E0 & selfKings) << 3) & noccupied;
    board |= ((0x0F0F0F0F & selfKings) << 4) & noccupied;

    while (board) {
        position = 0x80000000 >> __builtin_clz(board);
        odd = !(position & 0x0F0F0F0F);

        movelist.to[movelist.moveCount] = position;
        movelist.from[movelist.moveCount++] = position >> (3 + odd);

        board = board ^ position;
    }

    return movelist;
}

movelist_t get_other_moves() {
    int odd;

    movelist_t movelist;

    _board board,
           position,
           otherKings = gamestate.other & gamestate.kings,
           noccupied = ~gamestate.occupied;

    movelist.moveCount = 0;

    //North-east
    board = ((0xF0F0F0F0 & otherKings) >> 4) & noccupied;
    board |= ((0x07070700 & otherKings) >> 3) & noccupied;

    while (board) {
        position = 0x80000000 >> __builtin_clz(board);
        odd = !(position & 0x0F0F0F0F);

        movelist.to[movelist.moveCount] = position;
        movelist.from[movelist.moveCount++] = position << (4 - odd);

        board = board ^ position;
    }

    //North-west
    board = ((0xE0E0E0E0 & otherKings) >> 5) & noccupied;
    board |= ((0x0F0F0F00 & otherKings) >> 4) & noccupied;

    while (board) {
        position = 0x80000000 >> __builtin_clz(board);
        odd = !(position & 0x0F0F0F0F);

        movelist.to[movelist.moveCount] = position;
        movelist.from[movelist.moveCount++] = position << (5 - odd);

        board = board ^ position;
    }

    //South-east
    board = ((0x00F0F0F0 & gamestate.other) << 4) & noccupied;
    board |= ((0x07070707 & gamestate.other) << 5) & noccupied;

    while (board) {
        position = 0x80000000 >> __builtin_clz(board);
        odd = !(position & 0x0F0F0F0F);

        movelist.to[movelist.moveCount] = position;
        movelist.from[movelist.moveCount++] = position >> (4 + odd);

        board = board ^ position;
    }

    //South-west
    board = ((0x00E0E0E0 & gamestate.other) << 3) & noccupied;
    board |= ((0x0F0F0F0F & gamestate.other) << 4) & noccupied;

    while (board) {
        position = 0x80000000 >> __builtin_clz(board);
        odd = !(position & 0x0F0F0F0F);

        movelist.to[movelist.moveCount] = position;
        movelist.from[movelist.moveCount++] = position >> (3 + odd);

        board = board ^ position;
    }

    return movelist;
}

void do_jumps(jump_t jump, _board *self, _board *other) {
    _board pos = 0;
    int odd = !!(jump.from & 0x0F0F0F0F),
        i,
        west;

    *self = *self ^ (jump.from | jump.to);
    if (gamestate.kings & jump.from) {
        gamestate.kings ^= jump.from | jump.to;
    }

    for (i = 1; i <= jump.nsteps; i++) {
        if (jump.steps[i-1] > jump.steps[i]) {
            west = (jump.steps[i-1] >> 7) > jump.steps[i];
            pos |= jump.steps[i-1] >> jumpModifiers[odd][2 | west][0];
        } else {
            west = (jump.steps[i-1] << 7) == jump.steps[i];
            pos |= jump.steps[i-1] << jumpModifiers[odd][west][0];
        }
    }
    gamestate.kings = gamestate.kings & ~pos;
    *other = *other ^ pos;

    gamestate.occupied = *self | *other;
}

void do_move(movelist_t moves, int moveIndex, _board *self) {
    *self = *self ^ (moves.from[moveIndex] | moves.to[moveIndex]);
    if (gamestate.kings & moves.from[moveIndex]) {
        gamestate.kings ^= moves.from[moveIndex] | moves.to[moveIndex];
    }

    gamestate.occupied ^= moves.from[moveIndex] | moves.to[moveIndex];
}
