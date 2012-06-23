#include "heuristics.h"

heuristic_t caclulate_heuristics(board_t board, int isPlayerWhite, int isItPlayersTurn) {
    int whiteModifier = (isPlayerWhite << 1) - 1,
        blackModifier = -whiteModifier,
        whiteCount = POPULATION_COUNT(board.white),
        blackCount = POPULATION_COUNT(board.black),
        whiteKingCount = POPULATION_COUNT(board.white & board.kings),
        blackKingCount = POPULATION_COUNT(board.black & board.kings);

    // Turn bonus
    heuristic_t score = (isItPlayersTurn << 3) - (isItPlayersTurn << 1) - 3; //isItPlayersTurn*6-3
    
    // Material + king bonus
    score += whiteModifier * whiteCount * 100;
    score += blackModifier * blackCount * 100;
    score += whiteModifier * whiteKingCount * 50;
    score += blackModifier * blackKingCount* 50;

    // Back row bonus
    score += POPULATION_COUNT(0xF0000000 | board.occupied) * 7 * whiteModifier;
    score += POPULATION_COUNT(0x0000000F | board.occupied) * 7 * blackModifier;

    // Dog hole penalty
    score += (GET_BIT(board.black, 27) && GET_BIT(board.white, 31)) * 10 * whiteModifier;
    score += (GET_BIT(board.white, 4) && GET_BIT(board.black, 0)) * 10 * blackModifier;

    return score;
}
