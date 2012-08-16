#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "checkerboard.h"
#include "heuristics.h"
#include "moves.h"
#include "search.h"

board_t gamestate;
unsigned long long nodesVisited = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
    gamestate = input2board(argv[1]);

    pthread_t thread;

    struct timespec sleepTime, remaining;

    sleepTime.tv_sec = 9;
    sleepTime.tv_nsec = 850000000;

    pthread_create(&thread, NULL, runSearch, NULL);

    nanosleep(&sleepTime, &remaining);

    pthread_mutex_lock(&mutex);
    pthread_kill(thread, SIGSTOP);

    if (global_bestJump != NULL) {
        printJump(*global_bestJump, argv[1][33] == 'O');
    } else {
        printMove(*global_bestFrom, *global_bestTo, argv[1][33] == 'O');
    }

    printf("\n%u nodes visited\n", nodesVisited);
    printf("%d depth reached\n", depthSearched);

    return EXIT_SUCCESS;
}
