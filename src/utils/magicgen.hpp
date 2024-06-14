//
// Created by Marcel on 08.03.2024.
//

#ifndef CHESSENGINE_MAGICGEN_HPP
#define CHESSENGINE_MAGICGEN_HPP

#include <cstdint>

// Class for finding Magic Numbers for Rook and Bishop Magic Table
class MagicGen {

private:
    constexpr static const int BitTable[64] = {
            63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
            51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
            26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
            58, 20, 37, 17, 36, 8
    };

    static constexpr int RBits[64] = {
            12, 11, 11, 11, 11, 11, 11, 12,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            12, 11, 11, 11, 11, 11, 11, 12
    };

    static constexpr int BBits[64] = {
            6, 5, 5, 5, 5, 5, 5, 6,
            5, 5, 5, 5, 5, 5, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 5, 5, 5, 5, 5, 5,
            6, 5, 5, 5, 5, 5, 5, 6
    };

    static uint64_t random_uint64_t();

    static uint64_t random_uint64_t_fewbits();

    static int count_1s(uint64_t b);

    static int pop_1st_bit(uint64_t *bb);

    static uint64_t index_to_uint64_t(int index, int bits, uint64_t m);

    static uint64_t rmask(int sq);

    static uint64_t bmask(int sq);

    static uint64_t ratt(int sq, uint64_t block);

    static uint64_t batt(int sq, uint64_t block);

    static int transform(uint64_t b, uint64_t magic, int bits);

    static uint64_t find_magic(int sq, int m, int bishop);

public:
    static void run();
};

#endif
