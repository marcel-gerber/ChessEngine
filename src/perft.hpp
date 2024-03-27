//
// Created by Marcel on 22.03.2024.
//

#ifndef CHESSENGINE_PERFT_HPP
#define CHESSENGINE_PERFT_HPP

#include <cstdint>
#include <vector>
#include <chrono>
#include <iomanip>

#include "board.hpp"
#include "movegen.hpp"

class Perft {
private:
    Board &board;

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

public:
    explicit Perft(Board &board) : board(board) {

    }

    void run(int depth) {
        const auto t1 = std::chrono::high_resolution_clock::now();
        const auto nodes = perft(depth);
        const auto t2 = std::chrono::high_resolution_clock::now();
        const auto ms = duration_cast<std::chrono::milliseconds>(t2 - t1).count();

        std::stringstream stringstream;
        stringstream << "depth " << std::left << std::setw(2) << depth
           << " time " << std::setw(5) << ms
           << " nodes " << std::setw(12) << nodes
           << " nps " << std::setw(9) << (nodes * 1000) / (ms + 1);

        std::cout << stringstream.str() << std::endl;
    }
};

#endif
