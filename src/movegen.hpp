//
// Created by Marcel on 12.03.2024.
//

#ifndef CHESSENGINE_MOVEGEN_HPP
#define CHESSENGINE_MOVEGEN_HPP

#include "board.hpp"
#include "color.hpp"
#include "move.hpp"

#include <cstdint>
#include <tuple>
#include <vector>

class MoveGen {

private:
    static inline uint64_t SQUARES_BETWEEN[64][64] = { 0 };

    template<typename T>
    static void addMoveToMoveList(std::vector<Move> &moves, uint64_t &bb_from, T function);

    static constexpr uint64_t shift(uint64_t bits, const int8_t &direction) {
        switch(direction) {
            case 8:
                return bits << 8;
            case -8:
                return bits >> 8;
            default:
                return 0ULL;
        }
    }

public:
    static void initSquaresBetween();

    static uint64_t pinMaskHV(const Board &board, Color color);

    static uint64_t pinMaskDiagonal(const Board &board, Color color);

    static uint64_t attackedSquares(const Board &board, Color color);

    static std::tuple<uint64_t, uint8_t> checkMask(const Board &board, Color color);

    static void getPromotionMoves(std::vector<Move> &moves, const uint8_t &target_index, const int8_t &direction);

    static void generatePawnMoves(const Board &board, const Color &color, std::vector<Move> &moves, const uint64_t &pin_hv,
                                  const uint64_t &pin_d, const uint64_t &checkmask);

    static uint64_t generateKnightMoves(const uint8_t &index);

    static uint64_t generateBishopMoves(const uint8_t &index, const uint64_t &pin_d, const uint64_t &bb_occupied);

    static uint64_t generateRookMoves(const uint8_t &index, const uint64_t &pin_hv, const uint64_t &bb_occupied);

    static uint64_t generateQueenMoves(const uint8_t &index, const uint64_t &pin_hv, const uint64_t &pin_d,
                                       const uint64_t &bb_occupied);

    static uint64_t generateKingMoves(const uint8_t &index, const uint64_t &bb_attacked,
                                      const uint64_t &bb_movable_squares);

    static void generateCastleMoves(Board &board, const Color &color, std::vector<Move> &moves, const uint64_t &bb_attacked);

    static void legalMoves(Board &board, const Color &color, std::vector<Move> &moves);

};

#endif
