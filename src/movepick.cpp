//
// Created by Marcel on 03.09.2024.
//

#include "movepick.hpp"

#include <algorithm>

constexpr int16_t BEST_SCORE = 10000;

int16_t MovePicker::mvv_lva(const PieceType &pt_victim, const PieceType &pt_attacker) {
    return VICTIMS_VALUE[pt_victim.getIndex()] - ATTACKERS_VALUE[pt_attacker.getIndex()];
}

void MovePicker::scoreMoves(const Board &board, const TT::Entry &entry, std::vector<Move> &movelist) {
    Move pv_move = {};

    // Check if a move is in Transposition Table
    if(entry.zobrist_key == board.getZobrist()) {
        auto it = std::find(movelist.begin(), movelist.end(), entry.best_move);

        if(it != movelist.end()) {
            pv_move = *it;
        }
    }

    for(Move &move : movelist) {
        // Check for PV Move
        if(move == pv_move) {
            move.setScore(BEST_SCORE);
            continue;
        }

        // MVV - LVA
        PieceType pt_to = board.getPiece(move.to_index()).getType();

        if(pt_to.getValue() == PieceType::NONE) continue;

        PieceType pt_from = board.getPiece(move.from_index()).getType();

        move.setScore(mvv_lva(pt_to, pt_from));
    }
}

void MovePicker::sortMoves(std::vector<Move> &movelist) {
    std::sort(movelist.begin(), movelist.end(), [&](const Move &a, const Move &b) {
        return a.score() > b.score();
    });
}
