//
// Created by Marcel on 22.03.2024.
//

#ifndef CHESSENGINE_PERFT_HPP
#define CHESSENGINE_PERFT_HPP

#include <cstdint>
#include <vector>
#include "board.hpp"
#include "movegen.hpp"

class Perft {
private:
    Board board;

    uint64_t perft(int depth) {
        std::vector<Move> moves;
        MoveGen::legalMoves(board, board.getSideToMove(), moves);
    }
};

#endif
