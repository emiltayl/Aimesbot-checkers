#include "checkerboard.h"
#include "heuristics.h"

heuristic_t calculate_heuristics(board_t board, int isPlayerWhite, int isItPlayersTurn) {
    int whiteModifier = (isPlayerWhite << 1) - 1,
        blackModifier = -whiteModifier,
        whiteCount = population_count(board.white),
        blackCount = population_count(board.black),
        whiteKingCount = population_count(board.white & board.kings),
        blackKingCount = population_count(board.black & board.kings);

    // Turn bonus
    heuristic_t score = (isItPlayersTurn << 3) - (isItPlayersTurn << 1) - 3; //isItPlayersTurn*6-3
    
    // Material + king bonus
    score += whiteModifier * whiteCount * 100;
    score += blackModifier * blackCount * 100;
    score += whiteModifier * whiteKingCount * 50;
    score += blackModifier * blackKingCount* 50;

    // Runaway man
    score += ((0x00000010 & board.white) && !(0x00000010 & board.kings) && !(0x00000001 & board.occupied)) * 47 * whiteModifier;
    score += ((0x01000000 & board.black) && !(0x01000000 & board.kings) && !(0x30000000 & board.occupied)) * 47 * blackModifier;
    score += ((0x00000020 & board.white) && !(0x00000020 & board.kings) && !(0x00000003 & board.occupied)) * 47 * whiteModifier;
    score += ((0x02000000 & board.black) && !(0x02000000 & board.kings) && !(0x60000000 & board.occupied)) * 47 * blackModifier;
    score += ((0x00000040 & board.white) && !(0x00000040 & board.kings) && !(0x00000006 & board.occupied)) * 47 * whiteModifier;
    score += ((0x04000000 & board.black) && !(0x04000000 & board.kings) && !(0xC0000000 & board.occupied)) * 47 * blackModifier;
    score += ((0x00000080 & board.white) && !(0x00000080 & board.kings) && !(0x0000000C & board.occupied)) * 47 * whiteModifier;
    score += ((0x08000000 & board.black) && !(0x08000000 & board.kings) && !(0x80000000 & board.occupied)) * 47 * blackModifier;
    score += ((0x00000100 & board.white) && !(0x00000100 & board.kings) && !(0x00000011 & board.occupied)) * 44 * whiteModifier;
    score += ((0x00100000 & board.black) && !(0x00100000 & board.kings) && !(0x31000000 & board.occupied)) * 44 * blackModifier;
    score += ((0x00000200 & board.white) && !(0x00000200 & board.kings) && !(0x00000067 & board.occupied)) * 44 * whiteModifier;
    score += ((0x00200000 & board.black) && !(0x00200000 & board.kings) && !(0x73000000 & board.occupied)) * 44 * blackModifier;
    score += ((0x00000400 & board.white) && !(0x00000400 & board.kings) && !(0x000000CE & board.occupied)) * 44 * whiteModifier;
    score += ((0x00400000 & board.black) && !(0x00400000 & board.kings) && !(0xE6000000 & board.occupied)) * 44 * blackModifier;
    score += ((0x00000800 & board.white) && !(0x00000800 & board.kings) && !(0x0000008C & board.occupied)) * 44 * whiteModifier;
    score += ((0x00800000 & board.black) && !(0x00800000 & board.kings) && !(0x88000000 & board.occupied)) * 44 * blackModifier;

    // Back row bonus
    score += population_count(0xF0000000 & board.occupied) * 7 * whiteModifier;
    score += population_count(0x0000000F & board.occupied) * 7 * blackModifier;

    // Dog hole penalty
    score += (GET_BIT(board.black, 27) && GET_BIT(board.white, 31)) * 10 * whiteModifier;
    score += (GET_BIT(board.white, 4) && GET_BIT(board.black, 0)) * 10 * blackModifier;

    return score;
}
