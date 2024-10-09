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
    // Array containing the victims' value for each piece type
    static constexpr int16_t VICTIMS_VALUE[] = {
            Eval::PAWN_VALUE * 10,
            Eval::KNIGHT_VALUE * 10,
            Eval::BISHOP_VALUE * 10,
            Eval::ROOK_VALUE * 10,
            Eval::QUEEN_VALUE * 10
    };

    // Array containing the attackers' value for each piece type
    static constexpr int16_t ATTACKERS_VALUE[] = {
            Eval::PAWN_VALUE,
            Eval::KNIGHT_VALUE,
            Eval::BISHOP_VALUE,
            Eval::ROOK_VALUE,
            Eval::QUEEN_VALUE,
            Eval::KING_VALUE
    };

    /// Calculates a score based on most valuable victim - least valuable attacker
    static int16_t mvvLva(const PieceType &pt_victim, const PieceType &pt_attacker);

public:
    /// Scores all moves inside of 'movelist'
    static void scoreMoves(const Board &board, const TT::Entry* &entry, std::vector<Move> &movelist);

    /// Sorts all moves inside of 'movelist' beginning with highest score
    static void sortMoves(std::vector<Move> &movelist);

};

#endif