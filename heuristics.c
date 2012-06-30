#include "checkerboard.h"
#include "heuristics.h"

heuristic_t caclulate_heuristics(board_t board, int isPlayerWhite, int isItPlayersTurn) {
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

    // Back row bonus
    score += population_count(0xF0000000 | board.occupied) * 7 * whiteModifier;
    score += population_count(0x0000000F | board.occupied) * 7 * blackModifier;

    // Dog hole penalty
    score += (GET_BIT(board.black, 27) && GET_BIT(board.white, 31)) * 10 * whiteModifier;
    score += (GET_BIT(board.white, 4) && GET_BIT(board.black, 0)) * 10 * blackModifier;

    return score;
}
