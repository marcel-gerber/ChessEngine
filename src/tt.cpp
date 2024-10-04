//
// Created by Marcel on 23.08.2024.
//

#include "tt.hpp"

TT::Entry* TT::getEntry(const uint64_t &zobrist_key) {
    return &Table[zobrist_key & 0xFFFFF];
}

void TT::addEntry(const uint64_t &zobrist_key, const Move &best_move, const uint8_t &depth, const int &eval,
                     const int &alpha, const int &beta) {
    Entry* entry = getEntry(zobrist_key);
    Flag flag;

    if(eval <= alpha) {
        flag = Flag::UPPER_BOUND;
    } else if(eval >= beta) {
        flag = Flag::LOWER_BOUND;
    } else {
        flag = Flag::EXACT;
    }

    if(entry->zobrist_key != zobrist_key || flag == Flag::EXACT) {
        entry->zobrist_key = zobrist_key;
        entry->best_move = best_move;
        entry->depth = depth;
        entry->evaluation = eval;
        entry->flag = flag;
    }
}

void TT::clear() {
    std::fill(std::begin(Table), std::end(Table), Entry{});
}