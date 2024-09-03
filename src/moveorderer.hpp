//
// Created by Marcel on 03.09.2024.
//

#ifndef CHESSENGINE_MOVEORDERER_HPP
#define CHESSENGINE_MOVEORDERER_HPP

#include "board.hpp"
#include "eval.hpp"
#include "move.hpp"

#include <cstdint>
#include <vector>

class MoveOrderer {

private:
    static constexpr uint16_t VICTIMS_VALUE[] = {
            Eval::PAWN_VALUE * 10,
            Eval::KNIGHT_VALUE * 10,
            Eval::BISHOP_VALUE * 10,
            Eval::ROOK_VALUE * 10,
            Eval::QUEEN_VALUE * 10
    };

    static constexpr uint16_t ATTACKERS_VALUE[] = {
            Eval::PAWN_VALUE,
            Eval::KNIGHT_VALUE,
            Eval::BISHOP_VALUE,
            Eval::ROOK_VALUE,
            Eval::QUEEN_VALUE,
            Eval::KING_VALUE
    };

public:
    static void scoreMoves(const Board &board, const std::vector<Move> &movelist);

};

#endif