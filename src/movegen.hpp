//
// Created by Marcel on 12.03.2024.
//

#ifndef CHESSENGINE_MOVEGEN_HPP
#define CHESSENGINE_MOVEGEN_HPP

#include <cstdint>
#include "grid.hpp"
#include "attacks.hpp"

class MoveGen {
private:
    static inline uint64_t SQUARES_BETWEEN[64][64] = { 0 };

public:
    static void initSquaresBetween() {
        uint64_t squares_bb = 0ULL;
        uint64_t attacks = 0ULL;

        for(Square square1 = 0; square1 < 64; square1++) {
            for(Square square2 = 0; square2 < 64; square2++) {
                if(square1.getIndex() == square2.getIndex()) continue;

                squares_bb = (1ULL << square1.getIndex()) | (1ULL << square2.getIndex());

                if(square1.getFileIndex() == square2.getFileIndex()
                || square1.getRankIndex() == square2.getRankIndex()) {

                    attacks = Attacks::getRookAttacks(square1, squares_bb) & Attacks::getRookAttacks(square2, squares_bb);
                    SQUARES_BETWEEN[square1.getIndex()][square2.getIndex()] = attacks;

                    continue;
                }

                if(square1.getDiagonalIndex() == square2.getDiagonalIndex()
                || square1.getAntiDiagonalIndex() == square2.getAntiDiagonalIndex()) {

                    attacks = Attacks::getBishopAttacks(square1, squares_bb) & Attacks::getBishopAttacks(square2, squares_bb);
                    SQUARES_BETWEEN[square1.getIndex()][square2.getIndex()] = attacks;

                    continue;
                }
            }
        }
    }
};

#endif
