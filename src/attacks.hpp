//
// Created by Marcel on 07.03.2024.
//

#ifndef CHESSENGINE_ATTACKS_HPP
#define CHESSENGINE_ATTACKS_HPP

#include <cstdint>

class Attacks {
    // https://www.chessprogramming.org/Magic_Bitboards#Fancy
    struct Magic {
        uint64_t* attacks;
        uint64_t mask;
        uint64_t magic;
        uint8_t shit;

        uint32_t getIndex(uint64_t &bitboard) const {
            return ((bitboard & mask) * magic) >> shit;
        }
    };

    static Magic BishopTable[64];
    static Magic RookTable[64];
};

#endif
