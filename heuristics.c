#include "globals.h"
#include "checkerboard.h"
#include "heuristics.h"

/**
 * Calculates the heuristic score for the white player, and multiplies it with
 * the global variable whiteModifier.
 */
heuristic_t calculate_heuristics(int isItWhiteTurn) {
    int whiteCount = population_count(gamestate.white),
        blackCount = population_count(gamestate.black),
        whiteKingCount = population_count(gamestate.white & gamestate.kings),
        blackKingCount = population_count(gamestate.black & gamestate.kings);

    // Turn bonus
    heuristic_t score = (isItWhiteTurn << 3) - (isItWhiteTurn << 1) - 3; //isItWhiteTurn*6-3
    
    // Material + king bonus
    score += whiteCount * 100;
    score -= blackCount * 100;
    score += whiteKingCount * 50;
    score -= blackKingCount* 50;

    // Runaway man
    score += ((0x00000010 & (gamestate.white & (~gamestate.kings))) && !(0x00000001 & gamestate.occupied)) * 47;
    score -= ((0x01000000 & (gamestate.black & (~gamestate.kings))) && !(0x30000000 & gamestate.occupied)) * 47;
    score += ((0x00000020 & (gamestate.white & (~gamestate.kings))) && !(0x00000003 & gamestate.occupied)) * 47;
    score -= ((0x02000000 & (gamestate.black & (~gamestate.kings))) && !(0x60000000 & gamestate.occupied)) * 47;
    score += ((0x00000040 & (gamestate.white & (~gamestate.kings))) && !(0x00000006 & gamestate.occupied)) * 47;
    score -= ((0x04000000 & (gamestate.black & (~gamestate.kings))) && !(0xC0000000 & gamestate.occupied)) * 47;
    score += ((0x00000080 & (gamestate.white & (~gamestate.kings))) && !(0x0000000C & gamestate.occupied)) * 47;
    score -= ((0x08000000 & (gamestate.black & (~gamestate.kings))) && !(0x80000000 & gamestate.occupied)) * 47;
    score += ((0x00000100 & (gamestate.white & (~gamestate.kings))) && !(0x00000011 & gamestate.occupied)) * 44;
    score -= ((0x00100000 & (gamestate.black & (~gamestate.kings))) && !(0x31000000 & gamestate.occupied)) * 44;
    score += ((0x00000200 & (gamestate.white & (~gamestate.kings))) && !(0x00000067 & gamestate.occupied)) * 44;
    score -= ((0x00200000 & (gamestate.black & (~gamestate.kings))) && !(0x73000000 & gamestate.occupied)) * 44;
    score += ((0x00000400 & (gamestate.white & (~gamestate.kings))) && !(0x000000CE & gamestate.occupied)) * 44;
    score -= ((0x00400000 & (gamestate.black & (~gamestate.kings))) && !(0xE6000000 & gamestate.occupied)) * 44;
    score += ((0x00000800 & (gamestate.white & (~gamestate.kings))) && !(0x0000008C & gamestate.occupied)) * 44;
    score -= ((0x00800000 & (gamestate.black & (~gamestate.kings))) && !(0x88000000 & gamestate.occupied)) * 44;

    // Back row bonus
    score += population_count(0xF0000000 & gamestate.occupied) * 7;
    score -= population_count(0x0000000F & gamestate.occupied) * 7;

    // Dog hole penalty
    score += (GET_BIT(gamestate.black, 27) && GET_BIT(gamestate.white, 31)) * 10;
    score -= (GET_BIT(gamestate.white, 4) && GET_BIT(gamestate.black, 0)) * 10;

    return score * whiteModifier;
}
