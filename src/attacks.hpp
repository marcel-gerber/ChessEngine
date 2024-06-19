//
// Created by Marcel on 07.03.2024.
//

#ifndef CHESSENGINE_ATTACKS_HPP
#define CHESSENGINE_ATTACKS_HPP

#include "piece.hpp"
#include "grid.hpp"

#include <cstdint>

class Attacks {

private:
    // https://www.chessprogramming.org/Magic_Bitboards#Fancy
    struct Magic {
        uint64_t* attacks;
        uint64_t mask;
        uint64_t magic;
        uint8_t shift;

        [[nodiscard]] uint32_t getIndex(const uint64_t &bitboard) const {
            return ((bitboard & mask) * magic) >> shift;
        }
    };

    static inline Magic RookTable[64] = {};
    static inline Magic BishopTable[64] = {};

    static inline uint64_t RookAttacks[0x19000] = {};
    static inline uint64_t BishopAttacks[0x1480] = {};

    /// Function used for initializing Magic Table
    static uint64_t calculateRookAttacks(Square &square, uint64_t occupied);

    /// Function used for initializing Magic Table
    static uint64_t calculateBishopAttacks(Square &square, uint64_t occupied);

    static void initMagic(PieceType &&pieceType, Square &&square, Magic table[], uint64_t magic);

public:
    /// Get the pawns right-side attacks from the colors perspective
    static uint64_t getPawnRightAttacks(const uint64_t &bb_pawns, const Color &color);

    /// Get the pawns left-side attacks from the colors perspective
    static uint64_t getPawnLeftAttacks(const uint64_t &bb_pawns, const Color &color);

    static uint64_t getPawnAttacks(const Color &color, const uint8_t &index);

    static uint64_t getKnightAttacks(const uint8_t &index);

    static uint64_t getKingAttacks(const uint8_t &index);

    static uint64_t getRookAttacks(const uint8_t &index, const uint64_t &occupied);

    static uint64_t getBishopAttacks(const uint8_t &index, const uint64_t &occupied);

    static uint64_t getQueenAttacks(const uint8_t &index, const uint64_t &occupied);

    static void initMagics();
};

#endif
