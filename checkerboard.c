#include "checkerboard.h"

board_t input2board(char *input) {
    board_t board = {0, 0, 0, 0};

    board.black = (input[0] == 'x' || input[0] == 'X')
                | ((input[1] == 'x' || input[1] == 'X') << 1)
                | ((input[2] == 'x' || input[2] == 'X') << 2)
                | ((input[3] == 'x' || input[3] == 'X') << 3)
                | ((input[4] == 'x' || input[4] == 'X') << 4)
                | ((input[5] == 'x' || input[5] == 'X') << 5)
                | ((input[6] == 'x' || input[6] == 'X') << 6)
                | ((input[7] == 'x' || input[7] == 'X') << 7)
                | ((input[8] == 'x' || input[8] == 'X') << 8)
                | ((input[9] == 'x' || input[9] == 'X') << 9)
                | ((input[10] == 'x' || input[10] == 'X') << 10)
                | ((input[11] == 'x' || input[11] == 'X') << 11)
                | ((input[12] == 'x' || input[12] == 'X') << 12)
                | ((input[13] == 'x' || input[13] == 'X') << 13)
                | ((input[14] == 'x' || input[14] == 'X') << 14)
                | ((input[15] == 'x' || input[15] == 'X') << 15)
                | ((input[16] == 'x' || input[16] == 'X') << 16)
                | ((input[17] == 'x' || input[17] == 'X') << 17)
                | ((input[18] == 'x' || input[18] == 'X') << 18)
                | ((input[19] == 'x' || input[19] == 'X') << 19)
                | ((input[20] == 'x' || input[20] == 'X') << 20)
                | ((input[21] == 'x' || input[21] == 'X') << 21)
                | ((input[22] == 'x' || input[22] == 'X') << 22)
                | ((input[23] == 'x' || input[23] == 'X') << 23)
                | ((input[24] == 'x' || input[24] == 'X') << 24)
                | ((input[25] == 'x' || input[25] == 'X') << 25)
                | ((input[26] == 'x' || input[26] == 'X') << 26)
                | ((input[27] == 'x' || input[27] == 'X') << 27)
                | ((input[28] == 'x' || input[28] == 'X') << 28)
                | ((input[29] == 'x' || input[29] == 'X') << 29)
                | ((input[30] == 'x' || input[30] == 'X') << 30)
                | ((input[31] == 'x' || input[31] == 'X') << 31);

    board.white = (input[0] == 'o' || input[0] == 'O')
                | ((input[1] == 'o' || input[1] == 'O') << 1)
                | ((input[2] == 'o' || input[2] == 'O') << 2)
                | ((input[3] == 'o' || input[3] == 'O') << 3)
                | ((input[4] == 'o' || input[4] == 'O') << 4)
                | ((input[5] == 'o' || input[5] == 'O') << 5)
                | ((input[6] == 'o' || input[6] == 'O') << 6)
                | ((input[7] == 'o' || input[7] == 'O') << 7)
                | ((input[8] == 'o' || input[8] == 'O') << 8)
                | ((input[9] == 'o' || input[9] == 'O') << 9)
                | ((input[10] == 'o' || input[10] == 'O') << 10)
                | ((input[11] == 'o' || input[11] == 'O') << 11)
                | ((input[12] == 'o' || input[12] == 'O') << 12)
                | ((input[13] == 'o' || input[13] == 'O') << 13)
                | ((input[14] == 'o' || input[14] == 'O') << 14)
                | ((input[15] == 'o' || input[15] == 'O') << 15)
                | ((input[16] == 'o' || input[16] == 'O') << 16)
                | ((input[17] == 'o' || input[17] == 'O') << 17)
                | ((input[18] == 'o' || input[18] == 'O') << 18)
                | ((input[19] == 'o' || input[19] == 'O') << 19)
                | ((input[20] == 'o' || input[20] == 'O') << 20)
                | ((input[21] == 'o' || input[21] == 'O') << 21)
                | ((input[22] == 'o' || input[22] == 'O') << 22)
                | ((input[23] == 'o' || input[23] == 'O') << 23)
                | ((input[24] == 'o' || input[24] == 'O') << 24)
                | ((input[25] == 'o' || input[25] == 'O') << 25)
                | ((input[26] == 'o' || input[26] == 'O') << 26)
                | ((input[27] == 'o' || input[27] == 'O') << 27)
                | ((input[28] == 'o' || input[28] == 'O') << 28)
                | ((input[29] == 'o' || input[29] == 'O') << 29)
                | ((input[30] == 'o' || input[30] == 'O') << 30)
                | ((input[31] == 'o' || input[31] == 'O') << 31);

    board.kings = (input[0] == 'O' || input[0] == 'X')
                | ((input[1] == 'O' || input[1] == 'X') << 1)
                | ((input[2] == 'O' || input[2] == 'X') << 2)
                | ((input[3] == 'O' || input[3] == 'X') << 3)
                | ((input[4] == 'O' || input[4] == 'X') << 4)
                | ((input[5] == 'O' || input[5] == 'X') << 5)
                | ((input[6] == 'O' || input[6] == 'X') << 6)
                | ((input[7] == 'O' || input[7] == 'X') << 7)
                | ((input[8] == 'O' || input[8] == 'X') << 8)
                | ((input[9] == 'O' || input[9] == 'X') << 9)
                | ((input[10] == 'O' || input[10] == 'X') << 10)
                | ((input[11] == 'O' || input[11] == 'X') << 11)
                | ((input[12] == 'O' || input[12] == 'X') << 12)
                | ((input[13] == 'O' || input[13] == 'X') << 13)
                | ((input[14] == 'O' || input[14] == 'X') << 14)
                | ((input[15] == 'O' || input[15] == 'X') << 15)
                | ((input[16] == 'O' || input[16] == 'X') << 16)
                | ((input[17] == 'O' || input[17] == 'X') << 17)
                | ((input[18] == 'O' || input[18] == 'X') << 18)
                | ((input[19] == 'O' || input[19] == 'X') << 19)
                | ((input[20] == 'O' || input[20] == 'X') << 20)
                | ((input[21] == 'O' || input[21] == 'X') << 21)
                | ((input[22] == 'O' || input[22] == 'X') << 22)
                | ((input[23] == 'O' || input[23] == 'X') << 23)
                | ((input[24] == 'O' || input[24] == 'X') << 24)
                | ((input[25] == 'O' || input[25] == 'X') << 25)
                | ((input[26] == 'O' || input[26] == 'X') << 26)
                | ((input[27] == 'O' || input[27] == 'X') << 27)
                | ((input[28] == 'O' || input[28] == 'X') << 28)
                | ((input[29] == 'O' || input[29] == 'X') << 29)
                | ((input[30] == 'O' || input[30] == 'X') << 30)
                | ((input[31] == 'O' || input[31] == 'X') << 31);

    board.occupied = board.white | board.black;

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
