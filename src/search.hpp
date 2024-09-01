//
// Created by Marcel on 26.08.2024.
//

#ifndef CHESSENGINE_SEARCH_HPP
#define CHESSENGINE_SEARCH_HPP

#include "board.hpp"

class Search {

private:
    Board &board;
    int orig_depth = 0;
    Move best_move = {};
    int nodes_searched = 0;

    int negamax(int depth, int alpha, int beta);
    int quiescence(int alpha, int beta);

public:
    explicit Search(Board &board);

    int search(const int &depth);

    [[nodiscard]] Move getBestMove() const {
        return best_move;
    }

    [[nodiscard]] int getSearchedNodes() const {
        return nodes_searched;
    }
};

#endif
