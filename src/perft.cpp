#include "movegen.hpp"
#include "perft.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

Perft::Perft(Board &board) : board(board) {

}

[[nodiscard]] uint64_t Perft::perft(int depth) const {
    std::vector<Move> moves;
    MoveGen::legalMoves(board, moves);

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

void Perft::run(int depth) {
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

[[nodiscard]] uint64_t Perft::nodes(int depth) const {
    return perft(depth);
}
