#include <pthread.h>
#include <stdlib.h>

#include "checkerboard.h"
#include "globals.h"
#include "heuristics.h"
#include "moves.h"
#include "search.h"

jump_t *global_bestJump = NULL;
_board *global_bestFrom = NULL;
_board *global_bestTo   = NULL;
int depthSearched = 0;

void *runSearch(void *ptr) {
    int i, n, bestMoveIndex = 0, newBestMoveIndex = 0;
    jumplist_t jumpList;
    movelist_t moveList;
    board_t oldState = gamestate;
    heuristic_t alpha = HEURISTIC_MIN, beta = HEURISTIC_MAX, bestHeuristic, tmp;

    jumpList = get_self_jumps();
    if (jumpList.moveCount) {
        for (n = 3; n < 20; n++) {
            do_jumps(jumpList.moves[bestMoveIndex], &gamestate.self, &gamestate.other);
            bestHeuristic = betaSearch(n, alpha, beta);
            gamestate = oldState;

            for (i = 0; i < jumpList.moveCount; i++) {
                if (i == bestMoveIndex) {
                    continue;
                }

                do_jumps(jumpList.moves[bestMoveIndex], &gamestate.self, &gamestate.other);
                tmp = betaSearch(n, alpha, beta);
                if (tmp > bestHeuristic) {
                    bestHeuristic = tmp;
                    newBestMoveIndex = i;
                }
                gamestate = oldState;
            }

            pthread_mutex_lock(&mutex);
            bestMoveIndex = newBestMoveIndex;
            global_bestJump = &jumpList.moves[bestMoveIndex];
            depthSearched = n;
            pthread_mutex_unlock(&mutex);
            alpha = bestHeuristic - 500;
            beta = bestHeuristic + 500;
        }

        return NULL;
    }

    moveList = get_self_moves();
    for (n = 3; n < 20; n++) {
        do_move(moveList, bestMoveIndex, &gamestate.self);
        bestHeuristic = betaSearch(n, alpha, beta);
        gamestate = oldState;

        for (i = 0; i < moveList.moveCount; i ++) {
            if (i == bestMoveIndex) {
                continue;
            }

            do_move(moveList, i, &gamestate.self);
            tmp = betaSearch(n, alpha, beta);
            if (tmp > bestHeuristic) {
                bestHeuristic = tmp;
                newBestMoveIndex = i;
            }
            gamestate = oldState;
        }

        pthread_mutex_lock(&mutex);
        bestMoveIndex = newBestMoveIndex;
        global_bestFrom = &moveList.from[bestMoveIndex];
        global_bestTo = &moveList.to[bestMoveIndex];
        depthSearched = n;
        pthread_mutex_unlock(&mutex);
        alpha = bestHeuristic - 500;
        beta = bestHeuristic + 500;
    }

    return NULL;
}

heuristic_t alphaSearch(int depth, int alpha, int beta) {
    int i, tmp;
    jumplist_t jumpList;
    movelist_t moveList;
    board_t oldState = gamestate;

    nodesVisited++;

    jumpList = get_self_jumps();
    if (jumpList.moveCount) {
        for (i = 0; i < jumpList.moveCount; i++) {
            do_jumps(jumpList.moves[i], &gamestate.self, &gamestate.other);
            tmp = betaSearch(depth - 1, alpha, beta);
            alpha = (alpha < tmp) ? tmp : alpha;
            gamestate = oldState;

            if (alpha >= beta) {
                break;
            }
        }

        return alpha;
    }

    if (depth < 1) {
        return calculate_heuristics(1);
    }

    moveList = get_self_moves();

    if (moveList.moveCount == 0) {
        return HEURISTIC_MIN;
    }

    for (i = 0; i < moveList.moveCount; i++) {
        do_move(moveList, i, &gamestate.self);
        tmp = betaSearch(depth - 1, alpha, beta);
        alpha = (alpha < tmp) ? tmp : alpha;
        gamestate = oldState;

        if (alpha >= beta) {
            break;
        }
    }

    return alpha;
}

heuristic_t betaSearch(int depth, int alpha, int beta) {
    int i, tmp;
    jumplist_t jumpList;
    movelist_t moveList;
    board_t oldState = gamestate;

    nodesVisited++;

    jumpList = get_other_jumps();
    if (jumpList.moveCount) {
        for (i = 0; i < jumpList.moveCount; i++) {
            do_jumps(jumpList.moves[i], &gamestate.other, &gamestate.self);
            tmp = alphaSearch(depth - 1, alpha, beta);
            beta = (beta > tmp) ? tmp : beta;
            gamestate = oldState;

            if (alpha >= beta) {
                break;
            }
        }

        return beta;
    }

    if (depth < 1) {
        return calculate_heuristics(0);
    }

    moveList = get_other_moves();

    if (moveList.moveCount == 0) {
        return HEURISTIC_MAX;
    }

    for (i = 0; i < moveList.moveCount; i++) {
        do_move(moveList, i, &gamestate.other);
        tmp = alphaSearch(depth - 1, alpha, beta);
        beta = (beta > tmp) ? tmp : beta;
        gamestate = oldState;

        if (alpha >= beta) {
            break;
        }
    }

    return beta;
}
