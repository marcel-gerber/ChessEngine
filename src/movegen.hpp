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

    static uint64_t pinMaskHV(const Board &board, Color color, Square square, uint64_t bb_opp, uint64_t bb_us) {
        const uint64_t opp_rook = board.getPieces(color.getOppositeColor(), PieceType::ROOK);
        const uint64_t opp_queen = board.getPieces(color.getOppositeColor(), PieceType::QUEEN);

        uint64_t rook_attacks = Attacks::getRookAttacks(square, bb_opp) & (opp_rook | opp_queen);
        uint64_t pin_hv = 0ULL;

        while(rook_attacks) {
            uint8_t index = Misc::pop(rook_attacks);

            const uint64_t possible_pin = SQUARES_BETWEEN[square.getIndex()][index] | Square::toBitboard(index);
            if(Misc::popcount(possible_pin & bb_us) == 1) {
                pin_hv |= possible_pin;
            }
        }
        return pin_hv;
    }

    static uint64_t pinMaskDiagonal(const Board &board, Color color, Square square, uint64_t bb_opp, uint64_t bb_us) {
        const uint64_t opp_bishop = board.getPieces(color.getOppositeColor(), PieceType::BISHOP);
        const uint64_t opp_queen = board.getPieces(color.getOppositeColor(), PieceType::QUEEN);

        uint64_t bishop_attacks = Attacks::getBishopAttacks(square, bb_opp) & (opp_bishop | opp_queen);
        uint64_t pin_d = 0ULL;

        while(bishop_attacks) {
            uint8_t index = Misc::pop(bishop_attacks);

            const uint64_t possible_pin = SQUARES_BETWEEN[square.getIndex()][index] | Square::toBitboard(index);
            if(Misc::popcount(possible_pin & bb_us) == 1) {
                pin_d |= possible_pin;
            }
        }
        return pin_d;
    }

};

#endif
