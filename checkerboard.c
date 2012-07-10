#include "checkerboard.h"

board_t input2board(char *input) {
    board_t board = {0, 0, 0, 0};

    int selfMask = input[33] & 17; //bit 1 for white, 5 for black
    int otherMask = selfMask ^ 17;

    board.other = ((input[0] & otherMask) == otherMask)
                | (((input[1] & otherMask) == otherMask) << 1)
                | (((input[2] & otherMask) == otherMask) << 2)
                | (((input[3] & otherMask) == otherMask) << 3)
                | (((input[4] & otherMask) == otherMask) << 4)
                | (((input[5] & otherMask) == otherMask) << 5)
                | (((input[6] & otherMask) == otherMask) << 6)
                | (((input[7] & otherMask) == otherMask) << 7)
                | (((input[8] & otherMask) == otherMask) << 8)
                | (((input[9] & otherMask) == otherMask) << 9)
                | (((input[10] & otherMask) == otherMask) << 10)
                | (((input[11] & otherMask) == otherMask) << 11)
                | (((input[12] & otherMask) == otherMask) << 12)
                | (((input[13] & otherMask) == otherMask) << 13)
                | (((input[14] & otherMask) == otherMask) << 14)
                | (((input[15] & otherMask) == otherMask) << 15)
                | (((input[16] & otherMask) == otherMask) << 16)
                | (((input[17] & otherMask) == otherMask) << 17)
                | (((input[18] & otherMask) == otherMask) << 18)
                | (((input[19] & otherMask) == otherMask) << 19)
                | (((input[20] & otherMask) == otherMask) << 20)
                | (((input[21] & otherMask) == otherMask) << 21)
                | (((input[22] & otherMask) == otherMask) << 22)
                | (((input[23] & otherMask) == otherMask) << 23)
                | (((input[24] & otherMask) == otherMask) << 24)
                | (((input[25] & otherMask) == otherMask) << 25)
                | (((input[26] & otherMask) == otherMask) << 26)
                | (((input[27] & otherMask) == otherMask) << 27)
                | (((input[28] & otherMask) == otherMask) << 28)
                | (((input[29] & otherMask) == otherMask) << 29)
                | (((input[30] & otherMask) == otherMask) << 30)
                | (((input[31] & otherMask) == otherMask) << 31);

    board.self = ((input[0] & selfMask) == selfMask)
               | (((input[1] & selfMask) == selfMask) << 1)
               | (((input[2] & selfMask) == selfMask) << 2)
               | (((input[3] & selfMask) == selfMask) << 3)
               | (((input[4] & selfMask) == selfMask) << 4)
               | (((input[5] & selfMask) == selfMask) << 5)
               | (((input[6] & selfMask) == selfMask) << 6)
               | (((input[7] & selfMask) == selfMask) << 7)
               | (((input[8] & selfMask) == selfMask) << 8)
               | (((input[9] & selfMask) == selfMask) << 9)
               | (((input[10] & selfMask) == selfMask) << 10)
               | (((input[11] & selfMask) == selfMask) << 11)
               | (((input[12] & selfMask) == selfMask) << 12)
               | (((input[13] & selfMask) == selfMask) << 13)
               | (((input[14] & selfMask) == selfMask) << 14)
               | (((input[15] & selfMask) == selfMask) << 15)
               | (((input[16] & selfMask) == selfMask) << 16)
               | (((input[17] & selfMask) == selfMask) << 17)
               | (((input[18] & selfMask) == selfMask) << 18)
               | (((input[19] & selfMask) == selfMask) << 19)
               | (((input[20] & selfMask) == selfMask) << 20)
               | (((input[21] & selfMask) == selfMask) << 21)
               | (((input[22] & selfMask) == selfMask) << 22)
               | (((input[23] & selfMask) == selfMask) << 23)
               | (((input[24] & selfMask) == selfMask) << 24)
               | (((input[25] & selfMask) == selfMask) << 25)
               | (((input[26] & selfMask) == selfMask) << 26)
               | (((input[27] & selfMask) == selfMask) << 27)
               | (((input[28] & selfMask) == selfMask) << 28)
               | (((input[29] & selfMask) == selfMask) << 29)
               | (((input[30] & selfMask) == selfMask) << 30)
               | (((input[31] & selfMask) == selfMask) << 31);

    board.kings = ((input[0] & 32) >> 5)
                | ((input[1] & 32) >> 4)
                | ((input[2] & 32) >> 3)
                | ((input[3] & 32) >> 2)
                | ((input[4] & 32) >> 1)
                | (input[5] & 32)
                | ((input[6] & 32) << 1)
                | ((input[7] & 32) << 2)
                | ((input[8] & 32) << 3)
                | ((input[9] & 32) << 4)
                | ((input[10] & 32) << 5)
                | ((input[11] & 32) << 6)
                | ((input[12] & 32) << 7)
                | ((input[13] & 32) << 8)
                | ((input[14] & 32) << 9)
                | ((input[15] & 32) << 10)
                | ((input[16] & 32) << 11)
                | ((input[17] & 32) << 12)
                | ((input[18] & 32) << 13)
                | ((input[19] & 32) << 14)
                | ((input[20] & 32) << 15)
                | ((input[21] & 32) << 16)
                | ((input[22] & 32) << 17)
                | ((input[23] & 32) << 18)
                | ((input[24] & 32) << 19)
                | ((input[25] & 32) << 20)
                | ((input[26] & 32) << 21)
                | ((input[27] & 32) << 22)
                | ((input[28] & 32) << 23)
                | ((input[29] & 32) << 24)
                | ((input[30] & 32) << 25)
                | ((input[31] & 32) << 26);
    board.kings = ~board.kings;

    if (input[33] == 'X') {
        board.self = board_reverse(board.self);
        board.other = board_reverse(board.other);
        board.kings = board_reverse(board.kings);
    }
    board.occupied = board.self | board.other;

    return board;
}

unsigned int population_count(_board x) {
    // http://stackoverflow.com/questions/109023/best-algorithm-to-count-the-number-of-set-bits-in-a-32-bit-integer

    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
}

_board board_reverse(_board x) {
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return((x >> 16) | (x << 16));
}
