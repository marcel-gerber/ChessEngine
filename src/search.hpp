//
// Created by Marcel on 26.08.2024.
//

#ifndef CHESSENGINE_SEARCH_HPP
#define CHESSENGINE_SEARCH_HPP

#include "board.hpp"
#include "constants.hpp"

#include <array>

class Search {

private:
    Board &board;
    int orig_depth = 0;
    Move best_move = {};
    int nodes_searched = 0;

    // PV Nodes
    std::array<std::array<Move, Constants::MAX_PLY>, Constants::MAX_PLY> pv;
    std::array<int, Constants::MAX_PLY> pv_length;

    int negamax(int depth, int alpha, int beta, int ply);
    int quiescence(int alpha, int beta);
    void iterativeDeepening(int max_depth);

    void resetData();
    void printInfo(int depth, int seldepth, int score, int nodes, int time);

public:
    explicit Search(Board &board);

    void start(const int &depth);

    [[nodiscard]] Move getBestMove() const {
        return best_move;
    }

    [[nodiscard]] int getSearchedNodes() const {
        return nodes_searched;
    }
};

#endif
