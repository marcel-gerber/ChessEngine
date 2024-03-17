//
// Created by Marcel on 12.03.2024.
//

#ifndef CHESSENGINE_MOVEGEN_HPP
#define CHESSENGINE_MOVEGEN_HPP

#include <cstdint>
#include <tuple>
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

    static uint64_t pinMaskHV(const Board &board, Color color) {
        const uint8_t king_index = board.getKingIndex(color);
        const uint64_t bb_us = board.getSide(color);
        const uint64_t bb_opp = board.getSide(color.getOppositeColor());

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

    static uint64_t pinMaskDiagonal(const Board &board, Color color) {
        const uint8_t king_index = board.getKingIndex(color);
        const uint64_t bb_us = board.getSide(color);
        const uint64_t bb_opp = board.getSide(color.getOppositeColor());

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

    static std::tuple<uint64_t, uint8_t> checkMask(const Board &board, Color color) {
        uint8_t double_check = 0;
        Color opp_color = color.getOppositeColor();
        const uint8_t king_index = board.getKingIndex(color);

        const uint64_t opp_queen = board.getPieces(opp_color, PieceType::QUEEN);

        const uint64_t opp_pawns = board.getPieces(opp_color, PieceType::PAWN);
        const uint64_t opp_knights = board.getPieces(opp_color, PieceType::KNIGHT);
        const uint64_t opp_bishops = board.getPieces(opp_color, PieceType::BISHOP) | opp_queen;
        const uint64_t opp_rooks = board.getPieces(opp_color, PieceType::ROOK) | opp_queen;

        // knight checks
        uint64_t knight_attacks = Attacks::getKnightAttacks(king_index) & opp_knights;
        double_check += bool(knight_attacks);

        // initializing check_mask
        uint64_t check_mask = knight_attacks;

        // pawn checks
        uint64_t pawn_attacks = Attacks::getPawnAttacks(opp_color, king_index) & opp_pawns;
        double_check += bool(pawn_attacks);
        check_mask |= pawn_attacks;

        // bishop checks
        uint64_t bishop_attacks = Attacks::getBishopAttacks(king_index, board.getOccupancy()) & opp_bishops;

        if(bishop_attacks) {
            const uint8_t index = Bits::lsb(bishop_attacks);
            check_mask |= SQUARES_BETWEEN[king_index][index] | Square::toBitboard(index);
            double_check++;
        }

        // rook checks
        uint64_t rook_attacks = Attacks::getRookAttacks(king_index, board.getOccupancy()) & opp_rooks;

        if(rook_attacks) {
            if(Bits::popcount(rook_attacks) > 1) {
                return {check_mask, 2};
            }

            const uint8_t index = Bits::lsb(rook_attacks);
            check_mask |= SQUARES_BETWEEN[king_index][index] | Square::toBitboard(index);
            double_check++;
        }

        if(!check_mask) {
            return {0xFFFFFFFFFFFFFFFFULL, double_check};
        }

        return {check_mask, double_check};
    }

    static void generatePawnMoves() {
        // TODO
    }

    static uint64_t generateKnightMoves(const uint8_t &index) {
        return Attacks::getKnightAttacks(index);
    }

    static uint64_t generateBishopMoves(const uint8_t &index, const uint64_t &pin_d, const uint64_t &bb_occupied) {
        // If the bishop is pinned diagonally, it can only move on this diagonal
        if(pin_d & Square::toBitboard(index)) {
            return Attacks::getBishopAttacks(index, bb_occupied) & pin_d;
        }
        return Attacks::getBishopAttacks(index, bb_occupied);
    }

    static uint64_t generateRookMoves(const uint8_t &index, const uint64_t &pin_hv, const uint64_t &bb_occupied) {
        // If the rook is pinned horizontally or vertically, it can only move horizontal/vertical
        if(pin_hv & Square::toBitboard(index)) {
            return Attacks::getRookAttacks(index, bb_occupied) & pin_hv;
        }
        return Attacks::getRookAttacks(index, bb_occupied);
    }

    static uint64_t generateQueenMoves(const uint8_t &index, const uint64_t &pin_hv,
                                       const uint64_t &pin_d, const uint64_t &bb_occupied) {
        uint64_t bb_square = Square::toBitboard(index);

        if(pin_hv & bb_square) {
            return Attacks::getRookAttacks(index, bb_occupied) & pin_hv;
        }

        if(pin_d & bb_square) {
            return Attacks::getBishopAttacks(index, bb_occupied) & pin_d;
        }

        uint64_t moves = 0ULL;
        moves |= Attacks::getRookAttacks(index, bb_occupied);
        moves |= Attacks::getBishopAttacks(index, bb_occupied);

        return moves;
    }

    static uint64_t generateKingMoves(const uint8_t &index, const uint64_t &bb_attacked,
                                      const uint64_t &bb_movable_squares) {
        return Attacks::getKingAttacks(index) & bb_movable_squares & ~bb_attacked;
    }

    static uint64_t generateCastleMoves(Board &board, const Color &color, const uint64_t &bb_attacked) {
        const auto castling_rights = board.getCastlingRights();
        if(castling_rights->hasNoCastling()) return 0ULL;

        const uint8_t king_index = board.getKingIndex(color);
        uint64_t moves = 0ULL;

        for(const auto castle : Castling::getCastlings(color)) {
            if(!castling_rights->has(castle)) continue;

            const uint8_t end_king_index = Castling::getEndingKingIndex(castle);
            const uint8_t start_rook_index = Castling::getStartingRookIndex(castle);

            // Squares that have to be empty
            const uint64_t not_occ_path = SQUARES_BETWEEN[king_index][start_rook_index];

            // Squares that are not allowed to be attacked by the enemy
            const uint64_t not_attacked_path = SQUARES_BETWEEN[king_index][end_king_index];

            // Bitboard with all empty squares and squares that are not attacked by the enemy
            const uint64_t empty_not_attacked = ~board.getOccupancy() & ~bb_attacked;

            // If the squares that have to be empty are not empty -> continue
            if((not_occ_path & ~board.getOccupancy()) != not_occ_path) {
                continue;
            }

            // If the squares that the king moves through are attacked -> continue
            if((not_attacked_path & empty_not_attacked) != not_attacked_path) {
                continue;
            }

            // If the king is in check -> continue
            if((bb_attacked & Square::toBitboard(king_index)) != 0) {
                continue;
            }

            moves |= Square::toBitboard(start_rook_index);
        }

        return moves;
    }

};

#endif
