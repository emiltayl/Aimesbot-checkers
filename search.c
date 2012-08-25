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
    unsigned int branchMask = 0;
    int i, n, bestMoveIndex = 0, newBestMoveIndex = 0;
    jumplist_t jumpList;
    movelist_t moveList;
    board_t oldState = gamestate;
    heuristic_t alpha = HEURISTIC_MIN, beta = HEURISTIC_MAX, bestHeuristic, tmp;

    position_list = hash_table_create(200000000);

    jumpList = get_self_jumps();
    if (jumpList.moveCount) {
        n = 0;
        while(1) {
            do_jumps(jumpList.moves[bestMoveIndex], &gamestate.self, &gamestate.other);
            bestHeuristic = betaSearch(n, alpha, beta, &branchMask);
            gamestate = oldState;

            for (i = 0; i < jumpList.moveCount; i++) {
                if (i == bestMoveIndex) {
                    continue;
                }

                do_jumps(jumpList.moves[i], &gamestate.self, &gamestate.other);
                tmp = betaSearch(n, bestHeuristic, beta, &branchMask);
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
        bestHeuristic = betaSearch(n, alpha, beta, &branchMask);
        gamestate = oldState;

        for (i = 0; i < moveList.moveCount; i ++) {
            if (i == bestMoveIndex) {
                continue;
            }

            do_move(moveList, i, &gamestate.self);
            tmp = betaSearch(n, bestHeuristic, beta, &branchMask);
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

heuristic_t alphaSearch(int depth, heuristic_t alpha, heuristic_t beta, unsigned int *branchMask) {
    unsigned int cutoffMask = 0;
    int i, bestMove = 0;
    jumplist_t jumpList;
    movelist_t moveList;
    board_t oldState = gamestate;
    hash_table_list_t *listPtr;

    heuristic_t tmp;

    nodesVisited++;

    listPtr = hash_table_get_gamestate(position_list, HASH_TABLE_SELF_TURN);
    if (listPtr != NULL) {
        if (depth <= (listPtr->turnState & ~(HASH_TABLE_SELF_TURN | HASH_TABLE_BETA_CUTOFF))
            && (!(listPtr->turnState & HASH_TABLE_BETA_CUTOFF)
                || listPtr->score >= beta)
            && (!(listPtr->turnState & HASH_TABLE_ALPHA_CUTOFF)
                || (listPtr->score <= alpha))) {
            return listPtr->score;
        }

        bestMove = listPtr->bestMove;
    }

    jumpList = get_self_jumps();
    if (jumpList.moveCount) {
        do_jumps(jumpList.moves[bestMove], &gamestate.self, &gamestate.other);
        tmp = betaSearch(depth - 1, alpha, beta, branchMask);
        gamestate = oldState;

        if (tmp >= beta) {
            cutoffMask = HASH_TABLE_BETA_CUTOFF;
            alpha = beta;
            goto alphaAfterJumpSearch;
        }

        if (alpha < tmp) {
            alpha = tmp;
            cutoffMask = *branchMask;
        }

        for (i = (listPtr == NULL); i < jumpList.moveCount; i++) {
            if (listPtr != NULL && i == listPtr->bestMove) {
                continue;
            }

            do_jumps(jumpList.moves[i], &gamestate.self, &gamestate.other);

            tmp = betaSearch(depth - 1, alpha, beta, branchMask);
            gamestate = oldState;

            if (tmp >= beta) {
                cutoffMask = HASH_TABLE_BETA_CUTOFF;
                *branchMask = HASH_TABLE_BETA_CUTOFF;
                alpha = beta;
                break;
            }

            if (alpha < tmp) {
                cutoffMask = *branchMask;
                alpha = tmp;
                bestMove = i;
            }
        }
        alphaAfterJumpSearch:

        if (listPtr != NULL) {
            listPtr->turnState = HASH_TABLE_SELF_TURN | cutoffMask | (depth > 0 ? depth : 0);
            listPtr->bestMove = bestMove;
            listPtr->score = alpha;
        } else {
            hash_table_add_gamestate(position_list, HASH_TABLE_SELF_TURN | cutoffMask | (depth > 0 ? depth : 0), bestMove, alpha);
        }

        return alpha;
    }

    if (depth < 1) {
        *branchMask = 0;
        return calculate_heuristics(1);
    }

    moveList = get_self_moves();

    if (moveList.moveCount == 0) {
        *branchMask = 0;
        return HEURISTIC_LOSS + population_score() - depth;
    }

    do_move(moveList, bestMove, &gamestate.self);
    tmp = betaSearch(depth - 1, alpha, beta, branchMask);
    gamestate = oldState;

    if (tmp >= beta) {
        cutoffMask = HASH_TABLE_BETA_CUTOFF;
        *branchMask = HASH_TABLE_BETA_CUTOFF;
        alpha = beta;
        goto alphaAfterMoveSearch;
    }

    if (alpha < tmp) {
        cutoffMask = *branchMask;
        alpha = tmp;
    }

    for (i = (listPtr == NULL); i < moveList.moveCount; i++) {
        if (listPtr != NULL && i == listPtr->bestMove) {
            continue;
        }
        do_move(moveList, i, &gamestate.self);

        tmp = betaSearch(depth - 1, alpha, beta, branchMask);
        gamestate = oldState;

        if (tmp >= beta) {
            cutoffMask = HASH_TABLE_BETA_CUTOFF;
            *branchMask = HASH_TABLE_BETA_CUTOFF;
            alpha = beta;
            break;
        }

        if (alpha < tmp) {
            cutoffMask = *branchMask;
            alpha = tmp;
            bestMove = i;
        }
    }
    alphaAfterMoveSearch:

    if (listPtr != NULL) {
        listPtr->turnState = HASH_TABLE_SELF_TURN | cutoffMask | (depth > 0 ? depth : 0);
        listPtr->bestMove = bestMove;
        listPtr->score = alpha;
    } else {
        hash_table_add_gamestate(position_list, HASH_TABLE_SELF_TURN | cutoffMask | (depth > 0 ? depth : 0), bestMove, alpha);
    }

    return alpha;
}

heuristic_t betaSearch(int depth, heuristic_t alpha, heuristic_t beta, unsigned int *branchMask) {
    unsigned int cutoffMask = 0;
    int i, bestMove = 0;
    jumplist_t jumpList;
    movelist_t moveList;
    board_t oldState = gamestate;
    hash_table_list_t *listPtr;

    heuristic_t tmp;

    nodesVisited++;

    listPtr = hash_table_get_gamestate(position_list, 0);
    if (listPtr != NULL) {
        if (depth <= (listPtr->turnState & ~HASH_TABLE_ALPHA_CUTOFF)
            && (!(listPtr->turnState & HASH_TABLE_BETA_CUTOFF)
                || listPtr->score >= beta)
            && (!(listPtr->turnState & HASH_TABLE_ALPHA_CUTOFF)
                || (listPtr->score <= alpha))) {
            return listPtr->score;
        }

        bestMove = listPtr->bestMove;
    }

    jumpList = get_other_jumps();
    if (jumpList.moveCount) {
        do_jumps(jumpList.moves[bestMove], &gamestate.other, &gamestate.self);
        tmp = alphaSearch(depth - 1, alpha, beta, branchMask);
        gamestate = oldState;

        if (tmp <= alpha) {
            cutoffMask = HASH_TABLE_ALPHA_CUTOFF;
            *branchMask = HASH_TABLE_ALPHA_CUTOFF;
            beta = alpha;
            goto betaAfterJumpSearch;
        }

        if (beta > tmp) {
            cutoffMask = *branchMask;
            beta = tmp;
        }

        for (i = (listPtr == NULL); i < jumpList.moveCount; i++) {
            if (listPtr != NULL && i == listPtr->bestMove) {
                continue;
            }

            do_jumps(jumpList.moves[i], &gamestate.other, &gamestate.self);

            tmp = alphaSearch(depth - 1, alpha, beta, branchMask);
            gamestate = oldState;

            if (tmp <= alpha) {
                cutoffMask = HASH_TABLE_ALPHA_CUTOFF;
                *branchMask = HASH_TABLE_ALPHA_CUTOFF;
                beta = alpha;
                break;
            }

            if (beta > tmp) {
                cutoffMask = *branchMask;
                beta = tmp;
                bestMove = i;
            }
        }
        betaAfterJumpSearch:

        if (listPtr != NULL) {
            listPtr->turnState = cutoffMask | (depth > 0 ? depth : 0);
            listPtr->bestMove = bestMove;
            listPtr->score = beta;
        } else {
            hash_table_add_gamestate(position_list, cutoffMask | (depth > 0 ? depth : 0), bestMove, beta);
        }

        return beta;
    }

    if (depth < 1) {
        *branchMask = 0;
        return calculate_heuristics(0);
    }

    moveList = get_other_moves();

    if (moveList.moveCount == 0) {
        *branchMask = 0;
        return HEURISTIC_WIN + population_score() + depth;
    }

    do_move(moveList, bestMove, &gamestate.other);
    tmp = alphaSearch(depth - 1, alpha, beta, branchMask);
    gamestate = oldState;

    if (tmp <= alpha) {
        cutoffMask = HASH_TABLE_ALPHA_CUTOFF;
        *branchMask = HASH_TABLE_ALPHA_CUTOFF;
        beta = alpha;
        goto betaAfterMoveSearch;
    }

    if (beta > tmp) {
        cutoffMask = *branchMask;
        beta = tmp;
    }

    for (i = (listPtr == NULL); i < moveList.moveCount; i++) {
        if (listPtr != NULL && i == listPtr->bestMove) {
            continue;
        }

        do_move(moveList, i, &gamestate.other);
        tmp = alphaSearch(depth - 1, alpha, beta, branchMask);
        gamestate = oldState;

        if (tmp <= alpha) {
            cutoffMask = HASH_TABLE_ALPHA_CUTOFF;
            *branchMask = HASH_TABLE_ALPHA_CUTOFF;
            beta = alpha;
            break;
        }

        if (beta > tmp) {
            cutoffMask = *branchMask;
            beta = tmp;
            bestMove = i;
        }
    }
    betaAfterMoveSearch:

    if (listPtr != NULL) {
        listPtr->turnState = cutoffMask | (depth > 0 ? depth : 0);
        listPtr->bestMove = bestMove;
        listPtr->score = beta;
    } else {
        hash_table_add_gamestate(position_list, cutoffMask | (depth > 0 ? depth : 0), bestMove, beta);
    }

    return beta;
}
