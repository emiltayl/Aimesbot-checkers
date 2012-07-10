#include "globals.h"
#include "checkerboard.h"
#include "heuristics.h"

/**
 * Calculates the heuristic score for the AI
 */
heuristic_t calculate_heuristics(int isItSelfTurn) {
    int selfCount = population_count(gamestate.self),
        otherCount = population_count(gamestate.other),
        selfKingCount = population_count(gamestate.self & gamestate.kings),
        otherKingCount = population_count(gamestate.other & gamestate.kings);

    // Turn bonus
    heuristic_t score = (isItSelfTurn << 3) - (isItSelfTurn << 1) - 3; //isItSelfTurn*6-3

    // Material + king bonus
    score += selfCount * 100;
    score -= otherCount * 100;
    score += selfKingCount * 50;
    score -= otherKingCount* 50;

    // Runaway man
    score += ((0x00000010 & (gamestate.self & (~gamestate.kings))) && !(0x00000001 & gamestate.occupied)) * 47;
    score -= ((0x01000000 & (gamestate.other & (~gamestate.kings))) && !(0x30000000 & gamestate.occupied)) * 47;
    score += ((0x00000020 & (gamestate.self & (~gamestate.kings))) && !(0x00000003 & gamestate.occupied)) * 47;
    score -= ((0x02000000 & (gamestate.other & (~gamestate.kings))) && !(0x60000000 & gamestate.occupied)) * 47;
    score += ((0x00000040 & (gamestate.self & (~gamestate.kings))) && !(0x00000006 & gamestate.occupied)) * 47;
    score -= ((0x04000000 & (gamestate.other & (~gamestate.kings))) && !(0xC0000000 & gamestate.occupied)) * 47;
    score += ((0x00000080 & (gamestate.self & (~gamestate.kings))) && !(0x0000000C & gamestate.occupied)) * 47;
    score -= ((0x08000000 & (gamestate.other & (~gamestate.kings))) && !(0x80000000 & gamestate.occupied)) * 47;
    score += ((0x00000100 & (gamestate.self & (~gamestate.kings))) && !(0x00000011 & gamestate.occupied)) * 44;
    score -= ((0x00100000 & (gamestate.other & (~gamestate.kings))) && !(0x31000000 & gamestate.occupied)) * 44;
    score += ((0x00000200 & (gamestate.self & (~gamestate.kings))) && !(0x00000067 & gamestate.occupied)) * 44;
    score -= ((0x00200000 & (gamestate.other & (~gamestate.kings))) && !(0x73000000 & gamestate.occupied)) * 44;
    score += ((0x00000400 & (gamestate.self & (~gamestate.kings))) && !(0x000000CE & gamestate.occupied)) * 44;
    score -= ((0x00400000 & (gamestate.other & (~gamestate.kings))) && !(0xE6000000 & gamestate.occupied)) * 44;
    score += ((0x00000800 & (gamestate.self & (~gamestate.kings))) && !(0x0000008C & gamestate.occupied)) * 44;
    score -= ((0x00800000 & (gamestate.other & (~gamestate.kings))) && !(0x88000000 & gamestate.occupied)) * 44;

    // Back row bonus
    score += population_count(0xF0000000 & gamestate.occupied) * 7;
    score -= population_count(0x0000000F & gamestate.occupied) * 7;

    // Dog hole penalty
    score += (GET_BIT(gamestate.other, 27) && GET_BIT(gamestate.self, 31)) * 10;
    score -= (GET_BIT(gamestate.self, 4) && GET_BIT(gamestate.other, 0)) * 10;

    return score;
}
