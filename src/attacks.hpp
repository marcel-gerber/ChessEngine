//
// Created by Marcel on 07.03.2024.
//

#ifndef CHESSENGINE_ATTACKS_HPP
#define CHESSENGINE_ATTACKS_HPP

#include "piece.hpp"
#include "grid.hpp"

#include <cstdint>

class Attacks {

public:
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

    static void ray(const Square &square, const Direction::Value &direction, uint64_t &attacks_bb, const uint64_t &occupied);

    /// Function used for initializing Magic Table
    static uint64_t calculateRook(Square &square, uint64_t occupied);

    /// Function used for initializing Magic Table
    static uint64_t calculateBishop(Square &square, uint64_t occupied);

    static void initMagic(PieceType &&pieceType, Square &&square, Magic table[], uint64_t magic);

public:
    /// Get the pawns right-side attacks from the colors perspective
    static uint64_t pawnRight(const uint64_t &bb_pawns, const Color &color);

    /// Get the pawns left-side attacks from the colors perspective
    static uint64_t pawnLeft(const uint64_t &bb_pawns, const Color &color);

    static uint64_t pawn(const Color &color, const uint8_t &index);

    static uint64_t knight(const uint8_t &index);

    static uint64_t king(const uint8_t &index);

    static uint64_t rook(const uint8_t &index, const uint64_t &occupied);

    static uint64_t bishop(const uint8_t &index, const uint64_t &occupied);

    static uint64_t queen(const uint8_t &index, const uint64_t &occupied);

    static void initMagics();
};

#endif
