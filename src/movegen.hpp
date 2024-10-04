//
// Created by Marcel on 12.03.2024.
//

#ifndef CHESSENGINE_MOVEGEN_HPP
#define CHESSENGINE_MOVEGEN_HPP

#include "utils/bits.hpp"
#include "board.hpp"
#include "color.hpp"
#include "move.hpp"

#include <cstdint>
#include <tuple>
#include <vector>

enum class MoveGenType : uint8_t {
    ALL,
    CAPTURE,
    QUIET
};

class MoveGen {

private:
    static inline uint64_t SQUARES_BETWEEN[64][64] = { 0 };

    template<typename T>
    static void addMoveToMoveList(std::vector<Move> &moves, uint64_t &bb_from, T function) {
        while(bb_from) {
            const uint8_t index_from = Bits::pop(bb_from);
            uint64_t bb_moves = function(index_from);

            while(bb_moves) {
                const uint8_t index_to = Bits::pop(bb_moves);
                moves.push_back(Move::create<MoveType::NORMAL>(index_from, index_to));
            }
        }
    }

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

    template<Color::Value color>
    static uint64_t pinMaskHV(const Board &board);

    template<Color::Value color>
    static uint64_t pinMaskDiagonal(const Board &board);

    template<Color::Value color>
    static uint64_t attackedSquares(const Board &board);

    template<Color::Value color>
    static std::tuple<uint64_t, uint8_t> checkMask(const Board &board);

    static void promotionMoves(std::vector<Move> &moves, const uint8_t &target_index, const int8_t &direction);

    template<Color::Value color, MoveGenType moveGenType>
    static void generatePawnMoves(const Board &board, std::vector<Move> &moves, const uint64_t &pin_hv,
                                  const uint64_t &pin_d, const uint64_t &checkmask);

    static uint64_t generateKnightMoves(const uint8_t &index);

    static uint64_t generateBishopMoves(const uint8_t &index, const uint64_t &pin_d, const uint64_t &bb_occupied);

    static uint64_t generateRookMoves(const uint8_t &index, const uint64_t &pin_hv, const uint64_t &bb_occupied);

    static uint64_t generateQueenMoves(const uint8_t &index, const uint64_t &pin_hv, const uint64_t &pin_d,
                                       const uint64_t &bb_occupied);

    static uint64_t generateKingMoves(const uint8_t &index, const uint64_t &bb_attacked,
                                      const uint64_t &bb_movable_squares);

    template<Color::Value color, MoveGenType moveGenType>
    static void generateCastleMoves(const Board &board, std::vector<Move> &moves, const uint64_t &bb_attacked);

    template<Color::Value color, MoveGenType moveGenType>
    static void legalMoves(const Board &board, std::vector<Move> &moves);

    template<MoveGenType moveGenType>
    static void legalMoves(const Board &board, std::vector<Move> &moves);

};

#endif
