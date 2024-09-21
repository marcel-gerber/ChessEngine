//
// Created by Marcel on 23.08.2024.
//

#include "tt.hpp"

TT::Entry TT::getEntry(const uint64_t &zobrist_key) {
    return Table[zobrist_key & 0xFFFFF];
}

void TT::addEntry(const uint64_t &zobrist_key, const Move &best_move, const uint8_t &depth, const int &eval,
                     const int &alpha, const int &beta) {
    Entry entry{};
    entry.zobrist_key = zobrist_key;
    entry.best_move = best_move;
    entry.depth = depth;
    entry.evaluation = eval;

    if(eval <= alpha) {
        entry.nodeType = NodeType::ALL_NODE;
    } else if(eval >= beta) {
        entry.nodeType = NodeType::CUT_NODE;
    } else {
        entry.nodeType = NodeType::PV_NODE;
    }

    Table[zobrist_key & 0xFFFFF] = entry;
}

void TT::clear() {
    std::fill(std::begin(Table), std::end(Table), Entry{});
}