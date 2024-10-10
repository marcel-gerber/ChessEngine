#ifndef CHESSENGINE_PERFT_HPP
#define CHESSENGINE_PERFT_HPP

#include <cstdint>

#include "board.hpp"

class Perft {

private:
    Board &board;

    /// Returns number of all legal moves given the depth
    [[nodiscard]] uint64_t perft(int depth) const;

public:
    explicit Perft(Board &board);

    /// Runs perft test. Prints information when done
    void run(int depth);

    /// Returns number of all legal moves given the depth
    [[nodiscard]] uint64_t nodes(int depth) const;
};

#endif
