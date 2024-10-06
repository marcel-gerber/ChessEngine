//
// Created by Marcel on 23.08.2024.
//

#ifndef CHESSENGINE_TT_HPP
#define CHESSENGINE_TT_HPP

#include "move.hpp"

#include <cstdint>
#include <iostream>

enum class Flag : uint8_t {
    EXACT,          // PV-Node
    UPPER_BOUND,    // All-Node
    LOWER_BOUND     // Cut-Node
};

class TT {

public:
    struct Entry {
        uint64_t zobrist_key;
        Move best_move;
        uint8_t depth;
        int evaluation;
        Flag flag;
    };

    static Entry* getEntry(const uint64_t &zobrist_key);

    static void addEntry(const uint64_t &zobrist_key, const Move &best_move, const uint8_t &depth, const int &eval,
                         const int &alpha, const int &beta);

    static void clear();

private:
    static inline Entry Table[0xFFFFF] = {};

};

#endif