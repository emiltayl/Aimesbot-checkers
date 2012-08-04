#ifndef HASTABLE_H_
#define HASHTABLE_H_

#include "checkerboard.h"
#include "heuristics.h"

#define HASH_TABLE_GROW_SIZE 0.75

#define HASH_TABLE_SELF_TURN (1 << 31)

typedef struct _hash_table_list {
    board_t board;
    int turnState; //1 << 31 is set if it it's self's turn + depth left
    int bestMove;
    heuristic_t score;
    struct _hash_table_list *next;
} hash_table_list_t;

typedef struct _hash_table {
    int size;
    int n_elements;
    int exponent;
    int next_split;
    hash_table_list_t **elements;
} hash_table_t;

_board hash_get_gamestate_value(board_t);
unsigned int hash_table_get_gamestate_position(hash_table_t *, board_t);
hash_table_t *hash_table_create(int);
hash_table_list_t *hash_table_get_gamestate(hash_table_t *, int);
void hash_table_add_gamestate(hash_table_t *, int, int, heuristic_t);
void hash_table_free(hash_table_t *);

#endif /*HASHTABLE_H_*/
