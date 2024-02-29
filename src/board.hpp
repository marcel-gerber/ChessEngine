//
// Created by Marcel on 28.02.2024.
//

#ifndef CHESSENGINE_BOARD_HPP
#define CHESSENGINE_BOARD_HPP

#include <cstdint>
#include "color.hpp"
#include "piece.hpp"
#include "castling_rights.hpp"
#include "square.hpp"

class Board {
private:
    uint64_t bb_pieces[2][6];
    uint64_t bb_sides[2];

    CastlingRights castling_rights;
    Square en_passant_square;
    uint8_t half_move_counter;
    Color side_to_move;

public:
    Board() {
        castling_rights = CastlingRights();
        en_passant_square = Square::NONE;
        side_to_move = Color::WHITE;
    }

    CastlingRights* getCastlingRights() {
        return &castling_rights;
    }

    Square* getEnPassantSquare() {
        return &en_passant_square;
    }

    uint8_t getSideToMove() {
        return side_to_move.getValue();
    }
};

#endif
