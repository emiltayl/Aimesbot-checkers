#include <stdlib.h>
#include <stdio.h>

#include "checkerboard.h"
#include "heuristics.h"

board_t gamestate;
unsigned long long nodesVisited;
int whiteModifier;

int
main(int argc, char **argv) {
    nodesVisited = 0;
	gamestate = input2board("xxxxxx.xxx.x......o.oooo..oo.ooo");
    whiteModifier = 1;
    // (isPlayerWhite << 1) - 1

	printf
	    ("White pieces: %d\nBlack Pieces: %d\nWhite heuristic: %d\n",
	     population_count(gamestate.white),
         population_count(gamestate.black),
	     calculate_heuristics(0)
	    );

    whiteModifier = -whiteModifier;
    printf("Black heuristic: %d\n", calculate_heuristics(0));

	return EXIT_SUCCESS;
}
