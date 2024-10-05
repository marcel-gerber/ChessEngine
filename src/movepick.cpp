//
// Created by Marcel on 03.09.2024.
//

#include "movepick.hpp"

#include <algorithm>

constexpr int16_t BEST_SCORE = 10000;

int16_t MovePicker::mvvLva(const PieceType &pt_victim, const PieceType &pt_attacker) {
    return VICTIMS_VALUE[pt_victim.index()] - ATTACKERS_VALUE[pt_attacker.index()];
}

void MovePicker::scoreMoves(const Board &board, const TT::Entry* &entry, std::vector<Move> &movelist) {
    Move pv_move = {};

    // Check if a move is in Transposition Table
    if(entry->zobrist_key == board.getZobrist()) {
        auto it = std::find(movelist.begin(), movelist.end(), entry->best_move);

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
        PieceType pt_to = board.getPiece(move.toIndex()).type();

        if(pt_to.value() == PieceType::NONE) continue;

        PieceType pt_from = board.getPiece(move.fromIndex()).type();

        move.setScore(mvvLva(pt_to, pt_from));
    }
}

void MovePicker::sortMoves(std::vector<Move> &movelist) {
    std::sort(movelist.begin(), movelist.end(), [&](const Move &a, const Move &b) {
        return a.score() > b.score();
    });
}
