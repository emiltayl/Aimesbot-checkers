#include <stdlib.h>

#include "globals.h"
#include "hashtable.h"

_board masks[] = {
0xDD65DC90,
0x428E029E,
0xA9FB7952,
0x33857693,
0x5A00E80C,
0xAC912432,
0xD2FA0054,
0xBD23647E,
0xF1D97DFD,
0x44425838,
0x83FC5057,
0xD124CEC1,
0xBD8673FF,
0x5F7E019F,
0x8C0E3770,
0x65C654BF,
0x8763749F,
0x8828D019,
0xC1BEBE72,
0x679E8CD3,
0xBCD7FC9A,
0xC3A86540,
0x2093E68C,
0xEF266280,
0x71078C18,
0x068E92CD,
0xC00B1C8B,
0xE12C0A5E,
0x2A5E1E9D,
0x1713AD17,
0x88A978B5,
0x44EE74E5,
0x7AAD21C7,
0x3D99DFCC,
0x87660599,
0xC73A51E9,
0xB2BECF34,
0xEBEB56A6,
0xCB5D5B19,
0x8EC5D3B7,
0xEA701E41,
0x5CCC81C3,
0x5B501F29,
0x716740DB,
0x18CB3BDA,
0x5DF9CF64,
0x47CC9980,
0x65CEE965,
0xB6FC643A,
0xD0137FAC,
0xCAF933CF,
0x0EC58789,
0xBE220DA5,
0xC5250500,
0x4E704254,
0x372E1C8A,
0x16327EB1,
0xF6C92FC4,
0x9C0EAA34,
0x3206CCD3,
0x95794830,
0x62A5261D,
0x057F9737,
0x73A6CC66,
0x1BA53000,
0x280E308A,
0x6767D21C,
0xDE1652AF,
0xD38000BE,
0xA68FB23A,
0x7093D992,
0xE6562421,
0xE344C6CC,
0x3E199614,
0x5FE82CDB,
0xE55CE256,
0x42C5C68F,
0xF48EB2FC,
0x4D487AF6,
0x3CED802A,
0x93D5A1CA,
0x37B6A6D2,
0x4A0B69B0,
0xF10A5DDC,
0x071ACE08,
0xB4F0AB44,
0x11B717DF,
0x683F6221,
0xA63B7A6B,
0x231D3457,
0x3CF195D4,
0x64F510CC,
0x312E245B,
0x113C6B0A,
0xA89B7FD0,
0x3EB516FB,
0x2FCAA686,
0x00E9B198,
0x2B1E6822,
0xA9672163,
0xC27ABD6F,
0x2D408F93,
0xE07DDB8B,
0x077A14D4,
0x34223EEB,
0x7BD94C30,
0x07AE9FCA,
0xD043A94A,
0xE7C428EE,
0xC3672489,
0xDB7682D6,
0x18E36E11,
0xA9E7F9D1,
0x39D7D57B,
0xF6978320,
0x24A6B8BE,
0x8962FE90,
0x53699B85,
0x7CEF2970,
0x8A2F6F18,
0x155A7617,
0xFEF6C575,
0xF4608445,
0xECBBDC92,
0x17218931,
0x0E00285C,
0x60AA0F50,
0x17E67983,
0x9592DD90,
0xFFA66326,
0xA5383743,
0x4BACAB9B,
0x84951A9A,
0x8BD952A3,
0xDA846AD4,
0x1F9B67B4,
0xC9B268F5,
0xAA4DC984,
0x224DFDB0,
0xB3640414,
0x881ACBD9,
0x909D9B38,
0x490CC5EC,
0x2E37721A,
0xCBB4E3EB,
0x517514AD,
0xA66F1515,
0x90187E99,
0x0ACF58A9,
0xF9F0C4BE,
0x816A5A8A,
0x8F99D8CA,
0xFAB1E596,
0x612E7326,
0xBD793D8F,
0x92E4197E,
0x1E38B135,
0xB1CB0B52,
0x85C9FC85,
0x8B03551B
};

_board hash_get_gamestate_value(board_t state) {
    _board hash_value = 0, tmp;
    int i, n;

    for (i = 0; i < 32; i++) {
        tmp = 1 << i;
        n = (i << 2) + i;
        n += !!(state.self & tmp);
        n += (!!(state.other & tmp))*3;
        n += !!(state.kings & tmp);
        hash_value ^= masks[n];
    }

    return hash_value;
}

unsigned int hash_table_get_gamestate_position(hash_table_t *hash_table, board_t state) {
    _board hash_value = hash_get_gamestate_value(state);

    unsigned int position = hash_value % (1 << hash_table->exponent);
    if (position < hash_table->next_split) {
        position = hash_value % (1 << (hash_table->exponent + 1));
    }

    return position;
}

hash_table_t *hash_table_create(int size) {
    if (size < 1) {
        return NULL; //We don't want to have to deal with the special case of empty tables
    }
    int i;

    hash_table_t *hash_table = (hash_table_t *) malloc(sizeof(hash_table_t));
    if (hash_table == NULL) {
        return NULL;
    }

    hash_table->elements = (hash_table_list_t **) malloc(sizeof(hash_table_list_t *) * size);
    if (hash_table->elements == NULL) {
        free(hash_table);
        return NULL;
    }

    for (i = 0; i < size; i++) {
        hash_table->elements[i] = NULL;
    }

    hash_table->size = size;
    hash_table->exponent = 0;

    for (i = size; i > 1; i = i >> 1) {
        hash_table->exponent++;
    }

    hash_table->next_split = hash_table->size - (1 << hash_table->exponent);
    hash_table->n_elements = 0;

    return hash_table;
}

hash_table_list_t *hash_table_get_gamestate(hash_table_t *hash_table, int playerMask) {
    unsigned int position = hash_table_get_gamestate_position(hash_table, gamestate);
    hash_table_list_t *list = hash_table->elements[position];

    while (list != NULL) {
        if (list->board.self == gamestate.self
            && list->board.other == gamestate.other
            && list->board.kings == gamestate.kings
            && (list->turnState & HASH_TABLE_SELF_TURN) == (playerMask & HASH_TABLE_SELF_TURN)) {
            return list;
        }
        list = list->next;
    }

    return NULL;
}

void hash_table_add_gamestate(hash_table_t *hash_table, int turnState, int bestMove, heuristic_t score) {
    unsigned int position = hash_table_get_gamestate_position(hash_table, gamestate);
    hash_table_list_t *new_element = (hash_table_list_t *) malloc(sizeof(hash_table_list_t));
    if (new_element == NULL) {
        return;
    }

    new_element->board = gamestate;
    new_element->turnState = turnState;
    new_element->bestMove = bestMove;
    new_element->score = score;
    new_element->next = NULL;

    hash_table_list_t **list_element = &(hash_table->elements[position]);
    while (*list_element != NULL) {
        list_element = &((*list_element)->next);
    }

    *list_element = new_element;
    hash_table->n_elements++;

    if (((float) hash_table->n_elements/hash_table->size) > HASH_TABLE_GROW_SIZE) {
        //There are so many entries that we should grow the table
        hash_table_list_t **elements = (hash_table_list_t **) realloc(hash_table->elements, (hash_table->size + 1) * sizeof(hash_table_list_t *));
        if (elements == NULL) {
            //We couldn't increase the size of the list, maybe we should have
            //a way of handling errors?
            return;
        }
        hash_table->elements = elements;
        hash_table->elements[hash_table->size] = NULL;
        hash_table->size++;

        hash_table_list_t **old_element = &(hash_table->elements[hash_table->next_split]);
        hash_table_list_t **new_element2 = &(hash_table->elements[hash_table->size-1]);

        hash_table->next_split++;
        while (*old_element != NULL) {
            if (hash_table_get_gamestate_position(hash_table, (*old_element)->board) != hash_table->next_split - 1) {
                *new_element2 = *old_element;
                *old_element = (*old_element)->next;
                new_element2 = &((*new_element2)->next);
                *new_element2 = NULL;
            } else {
                old_element = &((*old_element)->next);
            }
        }

        if ((1 << (hash_table->exponent + 1)) <= hash_table->size) {
            hash_table->exponent++;
            hash_table->next_split = 0;
        }
    }

    return;
}

void hash_table_free(hash_table_t *hash_table) {
    int i;
    hash_table_list_t *list_item, *next;
    for (i = 0; i < hash_table->size; i++) {
        list_item = hash_table->elements[i];
        while (list_item != NULL) {
            next = list_item->next;
            free(list_item);
            list_item = next;
        }
    }
    free(hash_table->elements);
    free(hash_table);
}
