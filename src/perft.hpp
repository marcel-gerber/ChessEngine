//
// Created by Marcel on 22.03.2024.
//

#ifndef CHESSENGINE_PERFT_HPP
#define CHESSENGINE_PERFT_HPP

#include <cstdint>

#include "board.hpp"

class Perft {

private:
    Board &board;

    [[nodiscard]] uint64_t perft(int depth) const;

public:
    explicit Perft(Board &board);

    void run(int depth);

    [[nodiscard]] uint64_t nodes(int depth) const;
};

#endif
