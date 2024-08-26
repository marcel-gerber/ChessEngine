//
// Created by Marcel on 26.08.2024.
//

#ifndef CHESSENGINE_SEARCH_HPP
#define CHESSENGINE_SEARCH_HPP

#include "board.hpp"

class Search {

public:
    static int negamax(Board &board, int depth, int alpha, int beta);
};

#endif
