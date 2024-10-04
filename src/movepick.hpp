//
// Created by Marcel on 03.09.2024.
//

#ifndef CHESSENGINE_MOVEPICK_HPP
#define CHESSENGINE_MOVEPICK_HPP

#include "board.hpp"
#include "eval.hpp"
#include "move.hpp"
#include "tt.hpp"

#include <cstdint>
#include <vector>

class MovePicker {

private:
    static constexpr int16_t VICTIMS_VALUE[] = {
            Eval::PAWN_VALUE * 10,
            Eval::KNIGHT_VALUE * 10,
            Eval::BISHOP_VALUE * 10,
            Eval::ROOK_VALUE * 10,
            Eval::QUEEN_VALUE * 10
    };

    static constexpr int16_t ATTACKERS_VALUE[] = {
            Eval::PAWN_VALUE,
            Eval::KNIGHT_VALUE,
            Eval::BISHOP_VALUE,
            Eval::ROOK_VALUE,
            Eval::QUEEN_VALUE,
            Eval::KING_VALUE
    };

    static int16_t mvvLva(const PieceType &pt_victim, const PieceType &pt_attacker);

public:
    static void scoreMoves(const Board &board, const TT::Entry* &entry, std::vector<Move> &movelist);
    static void sortMoves(std::vector<Move> &movelist);

};

#endif