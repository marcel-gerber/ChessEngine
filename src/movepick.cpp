//
// Created by Marcel on 03.09.2024.
//

#include "movepick.hpp"

#include <algorithm>

int16_t MovePicker::mvv_lva(const PieceType &pt_victim, const PieceType &pt_attacker) {
    return VICTIMS_VALUE[pt_victim.getIndex()] - ATTACKERS_VALUE[pt_attacker.getIndex()];
}

void MovePicker::scoreMoves(const Board &board, std::vector<Move> &movelist) {
    // MVV - LVA
    for(Move &move : movelist) {
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
