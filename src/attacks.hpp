//
// Created by Marcel on 07.03.2024.
//

#ifndef CHESSENGINE_ATTACKS_HPP
#define CHESSENGINE_ATTACKS_HPP

#include <cstdint>
#include "piece.hpp"
#include "grid.hpp"
#include "misc/misc.hpp"
#include "constants.hpp"

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
    static uint64_t calculateRookAttacks(Square &square, uint64_t occupied) {
        uint64_t attacks = 0ULL;
        Square current{};

        // North-Direction
        current = Square(square.getIndex() + 8);
        while(current.isValid()) {
            attacks |= 1ULL << current.getIndex();
            if(Misc::isSet(occupied, current.getIndex())) break;
            current = Square(current.getIndex() + 8);
        }

        // East-Direction
        current = Square(square.getIndex() + 1);
        while(current.getFileIndex() != 0 && current.isValid()) {
            attacks |= 1ULL << current.getIndex();
            if(Misc::isSet(occupied, current.getIndex())) break;
            current = Square(current.getIndex() + 1);
        }

        // South-Direction
        current = Square(square.getIndex() - 8);
        while(current.isValid()) {
            attacks |= 1ULL << current.getIndex();
            if(Misc::isSet(occupied, current.getIndex())) break;
            current = Square(current.getIndex() - 8);
        }

        // West-Direction
        current = Square(square.getIndex() - 1);
        while(current.getFileIndex() != 7 && current.isValid()) {
            attacks |= 1ULL << current.getIndex();
            if(Misc::isSet(occupied, current.getIndex())) break;
            current = Square(current.getIndex() - 1);
        }

        return attacks;
    }

    /// Function used for initializing Magic Table
    static uint64_t calculateBishopAttacks(Square &square, uint64_t occupied) {
        uint64_t attacks = 0ULL;
        Square current{};

        // North-East-Direction
        current = Square(square.getIndex() + 9);
        while(current.getFileIndex() != 0 && current.isValid()) {
            attacks |= 1ULL << current.getIndex();
            if(Misc::isSet(occupied, current.getIndex())) break;
            current = Square(current.getIndex() + 9);
        }

        // South-East-Direction
        current = Square(square.getIndex() - 7);
        while(current.getFileIndex() != 0 && current.isValid()) {
            attacks |= 1ULL << current.getIndex();
            if(Misc::isSet(occupied, current.getIndex())) break;
            current = Square(current.getIndex() - 7);
        }

        // South-West-Direction
        current = Square(square.getIndex() - 9);
        while(current.getFileIndex() != 7 && current.isValid()) {
            attacks |= 1ULL << current.getIndex();
            if(Misc::isSet(occupied, current.getIndex())) break;
            current = Square(current.getIndex() - 9);
        }

        // North-West-Direction
        current = Square(square.getIndex() + 7);
        while(current.getFileIndex() != 7 && current.isValid()) {
            attacks |= 1ULL << current.getIndex();
            if(Misc::isSet(occupied, current.getIndex())) break;
            current = Square(current.getIndex() + 7);
        }

        return attacks;
    }

    static void initMagic(PieceType &&pieceType, Square &&square, Magic table[], uint64_t magic) {
        const uint64_t edges = ((Rank::RANK_1BB | Rank::RANK_8BB) & ~Rank::getBitboard(square.getRankIndex()) |
                ((File::FILE_ABB | File::FILE_HBB) & ~File::getBitboard(square.getFileIndex())));

        Magic* entry = &table[square.getIndex()];
        entry->magic = magic;

        uint64_t mask = (pieceType.getValue() == PieceType::ROOK
                ? calculateRookAttacks(square, 0ULL) : calculateBishopAttacks(square, 0ULL));
        mask &= ~edges;

        entry->mask = mask;
        entry->shift = 64 - Misc::popcount(mask);

        auto indices = Misc::getIndices(mask);
        int numBlockers = 1 << indices.size();

        // point pointer of the next square to current pointer + numBlockers
        if(square.getIndex() + 1 < 64) {
            table[square.getIndex() + 1].attacks = entry->attacks + numBlockers;
        }

        uint64_t blocker = 0ULL;

        // calculate all blocker bitboards and add them to 'attacks'
        for(int i = 0; i < numBlockers; i++) {
            for(int j = 0; j < indices.size(); j++) {
                int bit = (i >> j) & 1;
                blocker |= (uint64_t) bit << indices[j];
            }
            // calculate attack with 'blocker' mask and add it to the 'attacks' magic table at the 'magic index'
            entry->attacks[table[square.getIndex()].getIndex(blocker)] = (pieceType.getValue() == PieceType::ROOK
                    ? calculateRookAttacks(square, blocker) : calculateBishopAttacks(square, blocker));
            blocker = 0ULL;
        }
    }

public:
    /// Get the pawns right-side attacks from the colors perspective
    static uint64_t getPawnRightAttacks(Board &board, Color &color) {
        uint64_t pawns = board.getPieces(color, PieceType::PAWN);
        return color == Color::WHITE ? (pawns << 9 & ~File::FILE_ABB) : (pawns >> 9 & ~File::FILE_HBB);
    }

    /// Get the pawns left-side attacks from the colors perspective
    static uint64_t getPawnLeftAttacks(Board &board, Color &color) {
        uint64_t pawns = board.getPieces(color, PieceType::PAWN);
        return color == Color::WHITE ? (pawns << 7 & ~File::FILE_HBB) : (pawns >> 7 & ~File::FILE_ABB);
    }

    static uint64_t getPawnAttacks(Color &color, Square &square) {
        return Constants::PawnAttacks[color.getValue()][square.getIndex()];
    }

    static uint64_t getKnightAttacks(Square &square) {
        return Constants::KnightAttacks[square.getIndex()];
    }

    static uint64_t getKingAttacks(Square &square) {
        return Constants::KingAttacks[square.getIndex()];
    }

    static uint64_t getRookAttacks(Square &square, const uint64_t &occupied) {
        return RookTable[square.getIndex()].attacks[RookTable[square.getIndex()].getIndex(occupied)];
    }

    static uint64_t getBishopAttacks(Square &square, const uint64_t &occupied) {
        return BishopTable[square.getIndex()].attacks[BishopTable[square.getIndex()].getIndex(occupied)];
    }

    static uint64_t getQueenAttacks(Square &square, const uint64_t &occupied) {
        return getRookAttacks(square, occupied) | getBishopAttacks(square, occupied);
    }

    static void initMagics() {
        // Point the pointer 'attacks' to an array where the attacks are being stored
        RookTable[0].attacks = RookAttacks;
        BishopTable[0].attacks = BishopAttacks;

        for(int i = 0; i < 64; i++) {
            initMagic(PieceType::ROOK, Square(i), RookTable, Constants::RookMagics[i]);
            initMagic(PieceType::BISHOP, Square(i), BishopTable, Constants::BishopMagics[i]);
        }
    }
};

#endif
