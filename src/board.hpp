//
// Created by Marcel on 28.02.2024.
//

#ifndef CHESSENGINE_BOARD_HPP
#define CHESSENGINE_BOARD_HPP

#include <cstdint>
#include "color.hpp"
#include "piece.hpp"

class Board {
private:
    std::uint64_t pieces_bb[2][6];
    std::uint64_t allPieces[2];
    std::uint64_t occupied;
    std::uint64_t enPassant;
    Color activePlayer;

public:
    Board() {
        activePlayer = Color::WHITE;
    }

};

#endif
