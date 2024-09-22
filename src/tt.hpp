//
// Created by Marcel on 23.08.2024.
//

#ifndef CHESSENGINE_TT_HPP
#define CHESSENGINE_TT_HPP

#include "move.hpp"

#include <cstdint>
#include <iostream>

enum class NodeType : uint8_t {
    PV_NODE,    // EXACT
    ALL_NODE,   // UPPER-BOUND
    CUT_NODE    // LOWER-BOUND
};

class TT {

public:
    struct Entry {
        uint64_t zobrist_key;
        Move best_move;
        uint8_t depth;
        int evaluation;
        NodeType nodeType;
    };

    static Entry* getEntry(const uint64_t &zobrist_key);

    static void addEntry(const uint64_t &zobrist_key, const Move &best_move, const uint8_t &depth, const int &eval,
                         const int &alpha, const int &beta);

    static void clear();

private:
    static inline Entry Table[0xFFFFF] = {};

};

#endif