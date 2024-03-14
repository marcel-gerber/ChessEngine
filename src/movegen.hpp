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

                    attacks = Attacks::getRookAttacks(square1.getIndex(), squares_bb)
                            & Attacks::getRookAttacks(square2.getIndex(), squares_bb);
                    SQUARES_BETWEEN[square1.getIndex()][square2.getIndex()] = attacks;

                    continue;
                }

                if(square1.getDiagonalIndex() == square2.getDiagonalIndex()
                || square1.getAntiDiagonalIndex() == square2.getAntiDiagonalIndex()) {

                    attacks = Attacks::getBishopAttacks(square1.getIndex(), squares_bb)
                            & Attacks::getBishopAttacks(square2.getIndex(), squares_bb);
                    SQUARES_BETWEEN[square1.getIndex()][square2.getIndex()] = attacks;

                    continue;
                }
            }
        }
    }

    static uint64_t pinMaskHV(const Board &board, Color color, uint64_t bb_opp, uint64_t bb_us) {
        const uint8_t king_index = board.getKingIndex(color);

        const uint64_t opp_rook = board.getPieces(color.getOppositeColor(), PieceType::ROOK);
        const uint64_t opp_queen = board.getPieces(color.getOppositeColor(), PieceType::QUEEN);

        uint64_t rook_attacks = Attacks::getRookAttacks(king_index, bb_opp) & (opp_rook | opp_queen);
        uint64_t pin_hv = 0ULL;

        while(rook_attacks) {
            uint8_t index = Bits::pop(rook_attacks);

            const uint64_t possible_pin = SQUARES_BETWEEN[king_index][index] | Square::toBitboard(index);
            if(Bits::popcount(possible_pin & bb_us) == 1) {
                pin_hv |= possible_pin;
            }
        }
        return pin_hv;
    }

    static uint64_t pinMaskDiagonal(const Board &board, Color color, uint64_t bb_opp, uint64_t bb_us) {
        const uint8_t king_index = board.getKingIndex(color);

        const uint64_t opp_bishop = board.getPieces(color.getOppositeColor(), PieceType::BISHOP);
        const uint64_t opp_queen = board.getPieces(color.getOppositeColor(), PieceType::QUEEN);

        uint64_t bishop_attacks = Attacks::getBishopAttacks(king_index, bb_opp) & (opp_bishop | opp_queen);
        uint64_t pin_d = 0ULL;

        while(bishop_attacks) {
            uint8_t index = Bits::pop(bishop_attacks);

            const uint64_t possible_pin = SQUARES_BETWEEN[king_index][index] | Square::toBitboard(index);
            if(Bits::popcount(possible_pin & bb_us) == 1) {
                pin_d |= possible_pin;
            }
        }
        return pin_d;
    }

    static uint64_t attackedSquares(const Board &board, Color color) {
        const uint64_t occupied = board.getOccupancy();
        const uint64_t queens = board.getPieces(color, PieceType::QUEEN);
        uint64_t knights = board.getPieces(color, PieceType::KNIGHT);
        uint64_t bishops = board.getPieces(color, PieceType::BISHOP) | queens;
        uint64_t rooks = board.getPieces(color, PieceType::ROOK) | queens;

        uint64_t attacked = Attacks::getPawnLeftAttacks(board, color) | Attacks::getPawnRightAttacks(board, color);

        while(knights) {
            const uint8_t index = Bits::pop(knights);
            attacked |= Attacks::getKnightAttacks(index);
        }

        while(bishops) {
            const uint8_t index = Bits::pop(bishops);
            attacked |= Attacks::getBishopAttacks(index, occupied);
        }

        while(rooks) {
            const uint8_t index = Bits::pop(rooks);
            attacked |= Attacks::getRookAttacks(index, occupied);
        }

        const uint8_t king_index = board.getKingIndex(color);
        attacked |= Attacks::getKingAttacks(king_index);

        return attacked;
    }

};

#endif
