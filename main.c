#include <stdlib.h>
#include <stdio.h>

#include "checkerboard.h"
#include "heuristics.h"
#include "moves.h"
#include "search.h"

board_t gamestate;
unsigned long long nodesVisited = 0;

void printJump(jump_t jump, int isWhite) {
    int i;

    if (isWhite) {
        printf("%d", 32 - __builtin_clz(jump.steps[0]));

        for (i = 1; i <= jump.nsteps; i++) {
            printf("-%d", 32 - __builtin_clz(jump.steps[i]));
        }
    } else {
        printf("%d", 1 + __builtin_clz(jump.steps[0]));

        for (i = 1; i <= jump.nsteps; i++) {
            printf("-%d", 1 + __builtin_clz(jump.steps[i]));
        }
    }
}

void printMove(_board from, _board to, int isWhite) {
    if (isWhite) {
        printf("%d-%d", 32 - __builtin_clz(from), 32 - __builtin_clz(to));
    } else {
        printf("%d-%d", 1 + __builtin_clz(from), 1 + __builtin_clz(to));
    }
}

int main(int argc, char **argv) {
    nodesVisited = 0;
    gamestate = input2board(argv[1]);

    runSearch();

    if (global_bestJump != NULL) {
        printJump(*global_bestJump, argv[1][33] == 'O');
    } else {
        printMove(*global_bestFrom, *global_bestTo, argv[1][33] == 'O');
    }

    printf("\n%u nodes visited\n", nodesVisited);

    return EXIT_SUCCESS;
}
