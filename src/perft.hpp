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

        if(depth == 1) {
            return moves.size();
        }

        uint64_t nodes = 0ULL;

        for(const auto &move : moves) {
            board.makeMove(move);
            nodes += perft(depth - 1);
            board.unmakeMove(move);
        }

        return nodes;
    }
};

#endif
