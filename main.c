#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "checkerboard.h"
#include "heuristics.h"
#include "moves.h"

board_t gamestate;
unsigned long long nodesVisited;
int whiteModifier;

void printJump(jump_t move) {
    int i;

    printf("Jump from 0x%08X to 0x%08X\n", move.from, move.to);

    for (i = 0; i <= move.nsteps; i++) {
        printf("Position %d: %08X\n", i, move.steps[i]);
    }
}

void printJumps(jumplist_t movelist) {
    int i;

    printf("%d jump(s):\n", movelist.moveCount);

    for(i = 0; i < movelist.moveCount; i++) {
        printf("Jump #%d:\n", i);
        printJump(movelist.moves[i]);
    }
}

int compareStates(board_t state1, board_t state2) {
    return state1.other == state2.other
        && state1.self == state2.self
        && state1.kings == state2.kings
        && state1.occupied == state2.occupied;
}

void printMoves(movelist_t movelist) {
    int i;

    printf("%d move(s):\n", movelist.moveCount);

    for (i = 0; i < movelist.moveCount; i++) {
        printf("Move #%d from 0x%08X to 0x%08X\n", i, movelist.from[i], movelist.to[i]);
    }
}

void printState(board_t state) {
    int i = 0, n;
    char pieces[] = {'o', 'x', 'O', 'X'};

    printf("#========#\n");
    while (i < 32) {
        printf("|");
        for (n = 0; n < 4; n++, i++) {
            printf("\033[47m \033[0m\033[41m");
            if (state.occupied & (1 << i)) {
                if (state.kings & (1 << i)) {
                    printf("\033[1m");
                }

                if (state.self & (1 << i)) {
                    printf("\033[37m");
                } else {
                    printf("\033[30m");
                }

                printf("%c", pieces[(!!(state.other & (1 << i))) | ((!!(state.kings & (1 << i))) << 1)]);

            } else {
                if (state.kings & (1 << i)) {
                    printf("E");
                } else {
                    printf(" ");
                }
            }
            printf("\033[0m");
        }

        printf("|\n|");

        for (n = 0; n < 4; n++, i++) {
            printf("\033[41m");
            if (state.occupied & (1 << i)) {
                if (state.kings & (1 << i)) {
                    printf("\033[1m");
                }

                if (state.self & (1 << i)) {
                    printf("\033[37m");
                } else {
                    printf("\033[30m");
                }

                printf("%c", pieces[(!!(state.other & (1 << i))) | ((!!(state.kings & (1 << i))) << 1)]);

            } else {
                printf(" ");
            }
            printf("\033[47m \033[0m");
        }

        printf("|\n");
    }
    printf("#========#\n");
}

int main(int argc, char **argv) {
    nodesVisited = 0;
    gamestate = input2board("xxXxx..xxx.x....x.o.o.ooX.oxOoo.,O");

    board_t initialState = gamestate;

    printState(gamestate);

    printf
        ("Self pieces: %d\nOther Pieces: %d\nKings: %d\nSelf heuristic: %d\n",
         population_count(gamestate.self),
         population_count(gamestate.other),
         population_count(gamestate.kings),
         calculate_heuristics(0)
        );

    printf("Other heuristic: %d\n", -calculate_heuristics(0));

    jumplist_t selfJumps = get_self_jumps();
    printJumps(selfJumps);

    assert(compareStates(gamestate, initialState));

    jumplist_t otherJumps = get_other_jumps();
    printJumps(otherJumps);

    assert(compareStates(gamestate, initialState));

    movelist_t selfMoves = get_self_moves();
    printMoves(selfMoves);

    movelist_t otherMoves = get_other_moves();
    printMoves(otherMoves);

    assert(compareStates(gamestate, initialState));

    do_jumps(selfJumps.moves[0], &gamestate.self, &gamestate.other);

    printState(gamestate);

    do_move(otherMoves, 6, &gamestate.other);

    printState(gamestate);

    return EXIT_SUCCESS;
}
