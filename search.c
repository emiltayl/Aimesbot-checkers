#include <pthread.h>
#include <stdlib.h>

#include "checkerboard.h"
#include "globals.h"
#include "hashtable.h"
#include "heuristics.h"
#include "moves.h"
#include "search.h"

jump_t *global_bestJump = NULL;
_board *global_bestFrom = NULL;
_board *global_bestTo   = NULL;
int depthSearched = 0;

hash_table_t *position_list;

void *runSearch(void *ptr) {
    int i, n, bestMoveIndex = 0, newBestMoveIndex = 0;
    jumplist_t jumpList;
    movelist_t moveList;
    board_t oldState = gamestate;
    heuristic_t alpha = HEURISTIC_MIN, beta = HEURISTIC_MAX, bestHeuristic, tmp;

    position_list = hash_table_create(1500000);

    jumpList = get_self_jumps();
    if (jumpList.moveCount) {
        n = 0;
        while(1) {
            do_jumps(jumpList.moves[bestMoveIndex], &gamestate.self, &gamestate.other);
            bestHeuristic = betaSearch(n, alpha, beta);
            gamestate = oldState;

            for (i = 0; i < jumpList.moveCount; i++) {
                if (i == bestMoveIndex) {
                    continue;
                }

                do_jumps(jumpList.moves[i], &gamestate.self, &gamestate.other);
                tmp = betaSearch(n, bestHeuristic, beta);
                if (tmp > bestHeuristic) {
                    bestHeuristic = tmp;
                    newBestMoveIndex = i;
                }
                gamestate = oldState;
            }

            if (bestHeuristic <= alpha || bestHeuristic >= beta) {
                alpha = HEURISTIC_MIN;
                beta = HEURISTIC_MAX;
                continue;
            }

            bestMoveIndex = newBestMoveIndex;
            pthread_mutex_lock(&mutex);
            global_bestJump = &jumpList.moves[bestMoveIndex];
            depthSearched = n;
            pthread_mutex_unlock(&mutex);
            alpha = bestHeuristic - 150;
            beta = bestHeuristic + 150;
            n++;
        }

        return NULL;
    }

    moveList = get_self_moves();
    n = 0;
    while (1) {
        do_move(moveList, bestMoveIndex, &gamestate.self);
        bestHeuristic = betaSearch(n, alpha, beta);
        gamestate = oldState;

        for (i = 0; i < moveList.moveCount; i ++) {
            if (i == bestMoveIndex) {
                continue;
            }

            do_move(moveList, i, &gamestate.self);
            tmp = betaSearch(n, bestHeuristic, beta);
            if (tmp > bestHeuristic) {
                bestHeuristic = tmp;
                newBestMoveIndex = i;
            }
            gamestate = oldState;
        }

        if (bestHeuristic <= alpha || bestHeuristic >= beta) {
            alpha = HEURISTIC_MIN;
            beta = HEURISTIC_MAX;
            continue;
        }

        bestMoveIndex = newBestMoveIndex;
        pthread_mutex_lock(&mutex);
        global_bestFrom = &moveList.from[bestMoveIndex];
        global_bestTo = &moveList.to[bestMoveIndex];
        depthSearched = n;
        pthread_mutex_unlock(&mutex);
        alpha = bestHeuristic - 150;
        beta = bestHeuristic + 150;
        n++;
    }

    return NULL;
}

heuristic_t alphaSearch(int depth, heuristic_t alpha, heuristic_t beta) {
    int i, bestMove = 0;
    jumplist_t jumpList;
    movelist_t moveList;
    board_t oldState = gamestate;
    hash_table_list_t *listPtr;

    heuristic_t tmp;

    nodesVisited++;

    listPtr = hash_table_get_gamestate(position_list, HASH_TABLE_SELF_TURN);
    if (listPtr != NULL) {
        if (depth <= (listPtr->turnState & ~HASH_TABLE_SELF_TURN)) {
            return listPtr->score;
        }

        bestMove = listPtr->bestMove;
    }

    jumpList = get_self_jumps();
    if (jumpList.moveCount) {
        do_jumps(jumpList.moves[bestMove], &gamestate.self, &gamestate.other);
        tmp = betaSearch(depth - 1, alpha, beta);
        alpha = (alpha < tmp) ? tmp : alpha;
        gamestate = oldState;

        if (alpha >= beta) {
            goto alphaAfterJumpSearch;
        }

        for (i = 0; i < jumpList.moveCount; i++) {
            if (listPtr != NULL && i == listPtr->bestMove) {
                continue;
            }

            do_jumps(jumpList.moves[i], &gamestate.self, &gamestate.other);

            tmp = betaSearch(depth - 1, alpha, beta);

            if (alpha < tmp) {
                alpha = tmp;
                bestMove = i;
            }

            gamestate = oldState;

            if (alpha >= beta) {
                break;
            }
        }
        alphaAfterJumpSearch:

        if (listPtr != NULL) {
            listPtr->turnState = HASH_TABLE_SELF_TURN | (depth > 0 ? depth : 0);
            listPtr->bestMove = bestMove;
            listPtr->score = alpha;
        } else {
            hash_table_add_gamestate(position_list, HASH_TABLE_SELF_TURN | (depth > 0 ? depth : 0), bestMove, alpha);
        }

        return alpha;
    }

    if (depth < 1) {
        return calculate_heuristics(1);
    }

    moveList = get_self_moves();

    if (moveList.moveCount == 0) {
        return HEURISTIC_LOSS + calculate_heuristics(1) - depth;
    }

    do_move(moveList, bestMove, &gamestate.self);
    tmp = betaSearch(depth - 1, alpha, beta);

    alpha = (alpha < tmp) ? tmp : alpha;
    gamestate = oldState;
    if (alpha >= beta) {
        goto alphaAfterMoveSearch;
    }

    for (i = 0; i < moveList.moveCount; i++) {
        if (listPtr != NULL && i == listPtr->bestMove) {
            continue;
        }
        do_move(moveList, i, &gamestate.self);

        tmp = betaSearch(depth - 1, alpha, beta);
        if (alpha < tmp) {
            alpha = tmp;
            bestMove = i;
        }
        gamestate = oldState;

        if (alpha >= beta) {
            break;
        }
    }
    alphaAfterMoveSearch:

    if (listPtr != NULL) {
        listPtr->turnState = HASH_TABLE_SELF_TURN | (depth > 0 ? depth : 0);
        listPtr->bestMove = bestMove;
        listPtr->score = alpha;
    } else {
        hash_table_add_gamestate(position_list, HASH_TABLE_SELF_TURN | (depth > 0 ? depth : 0), bestMove, alpha);
    }

    return alpha;
}

heuristic_t betaSearch(int depth, heuristic_t alpha, heuristic_t beta) {
    int i, bestMove = 0;
    jumplist_t jumpList;
    movelist_t moveList;
    board_t oldState = gamestate;
    hash_table_list_t *listPtr;

    heuristic_t tmp;

    nodesVisited++;

    listPtr = hash_table_get_gamestate(position_list, 0);
    if (listPtr != NULL) {
        if (depth <= listPtr->turnState) {
            return listPtr->score;
        }

        bestMove = listPtr->bestMove;
    }

    jumpList = get_other_jumps();
    if (jumpList.moveCount) {
        do_jumps(jumpList.moves[bestMove], &gamestate.other, &gamestate.self);
        tmp = alphaSearch(depth - 1, alpha, beta);
        beta = (beta > tmp) ? tmp : beta;
        gamestate = oldState;

        if (alpha >= beta) {
            goto betaAfterJumpSearch;
        }

        for (i = 0; i < jumpList.moveCount; i++) {
            if (listPtr != NULL && i == listPtr->bestMove) {
                continue;
            }

            do_jumps(jumpList.moves[i], &gamestate.other, &gamestate.self);

            tmp = alphaSearch(depth - 1, alpha, beta);

            if (beta > tmp) {
                beta = tmp;
                bestMove = i;
            }

            gamestate = oldState;

            if (alpha >= beta) {
                break;
            }
        }
        betaAfterJumpSearch:

        if (listPtr != NULL) {
            listPtr->turnState = (depth > 0 ? depth : 0);
            listPtr->bestMove = bestMove;
            listPtr->score = beta;
        } else {
            hash_table_add_gamestate(position_list, (depth > 0 ? depth : 0), bestMove, beta);
        }

        return beta;
    }

    if (depth < 1) {
        return calculate_heuristics(0);
    }

    moveList = get_other_moves();

    if (moveList.moveCount == 0) {
        return HEURISTIC_WIN + calculate_heuristics(0) + depth;
    }

    do_move(moveList, bestMove, &gamestate.other);
    tmp = alphaSearch(depth - 1, alpha, beta);

    beta = (beta > tmp) ? tmp : beta;
    gamestate = oldState;
    if (alpha >= beta) {
        goto betaAfterMoveSearch;
    }

    for (i = 0; i < moveList.moveCount; i++) {
        if (listPtr != NULL && i == listPtr->bestMove) {
            continue;
        }
        do_move(moveList, i, &gamestate.other);

        tmp = alphaSearch(depth - 1, alpha, beta);
        if (beta > tmp) {
            beta = tmp;
            bestMove = i;
        }
        gamestate = oldState;

        if (alpha >= beta) {
            break;
        }
    }
    betaAfterMoveSearch:

    if (listPtr != NULL) {
        listPtr->turnState = (depth > 0 ? depth : 0);
        listPtr->bestMove = bestMove;
        listPtr->score = beta;
    } else {
        hash_table_add_gamestate(position_list, (depth > 0 ? depth : 0), bestMove, beta);
    }

    return beta;
}
