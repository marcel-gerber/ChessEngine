//
// Created by Marcel on 28.02.2024.
//

#ifndef CHESSENGINE_BOARD_HPP
#define CHESSENGINE_BOARD_HPP

#include <cstdint>
#include <cassert>
#include "color.hpp"
#include "piece.hpp"
#include "castling_rights.hpp"
#include "square.hpp"

class Board {
private:
    uint64_t bb_pieces[6];
    uint64_t bb_sides[2];

    CastlingRights castling_rights;
    Square en_passant_square;
    uint8_t half_move_clock;
    Color side_to_move;

    void set(uint64_t &bits, uint8_t &index) {
        assert(index >= 0 && index <= 63);
        bits |= (1ULL << index);
    }

    void unset(uint64_t &bits, uint8_t &index) {
        assert(index >= 0 && index <= 63);
        bits &= ~(1ULL << index);
    }

public:
    Board() {
        castling_rights = CastlingRights();
        en_passant_square = Square::NONE;
        side_to_move = Color::WHITE;
    }

    void placePiece(Piece &piece, uint8_t &index) {
        set(bb_pieces[piece.getType().getValue()], index);
        set(bb_sides[piece.getColor().getValue()], index);
    }

    CastlingRights* getCastlingRights() {
        return &castling_rights;
    }

    Square* getEnPassantSquare() {
        return &en_passant_square;
    }

    void setSideToMove(Color color) {
        side_to_move = color;
    }

    uint8_t getSideToMove() {
        return side_to_move.getValue();
    }

    void setHalfMoveClock(const uint8_t &half_move) {
        this->half_move_clock = half_move;
    }

    [[nodiscard]] uint8_t getHalfMoveClock() const {
        return half_move_clock;
    }
};

#endif
