#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "checkerboard.h"
#include "heuristics.h"
#include "moves.h"

board_t gamestate;
unsigned long long nodesVisited;
int whiteModifier;

void printJump(move_t move) {
    int i;

    printf("Jump from 0x%08X to 0x%08X\n", move.from, move.to);

    for (i = 0; i <= move.nsteps; i++) {
        printf("Position %d: %08X\n", i, move.steps[i]);
    }
}

void printJumps(movelist_t movelist) {
    int i;

    printf("%d move(s):\n", movelist.moveCount);

    for(i = 0; i < movelist.moveCount; i++) {
        printf("Move #%d:\n", i);
        printJump(movelist.moves[i]);
    }
}

int compareStates(board_t state1, board_t state2) {
    return state1.other == state2.other
        && state1.self == state2.self
        && state1.kings == state2.kings
        && state1.occupied == state2.occupied;
}

int main(int argc, char **argv) {
    nodesVisited = 0;
    gamestate = input2board("xxxxxx.xxx.x..o...o.ooooX.oooooo,O");

    board_t initialState = gamestate;

    printf
        ("Self board: 0x%08X\nOther board: 0x%08X\nKings: 0x%08X\nOccupied: 0x%08X\n",
         gamestate.self,
         gamestate.other,
         gamestate.kings,
         gamestate.occupied
        );

    printf
        ("Self pieces: %d\nOther Pieces: %d\nSelf heuristic: %d\n",
         population_count(gamestate.self),
         population_count(gamestate.other),
         calculate_heuristics(0)
        );

    printf("Other heuristic: %d\n", -calculate_heuristics(0));

    movelist_t selfMoves = get_self_jumps();
    printJumps(selfMoves);

    assert(compareStates(gamestate, initialState));

    movelist_t otherMoves = get_other_jumps();
    printJumps(otherMoves);

    assert(compareStates(gamestate, initialState));

    return EXIT_SUCCESS;
}
