//
// Created by Marcel on 28.02.2024.
//

#ifndef CHESSENGINE_PIECE_HPP
#define CHESSENGINE_PIECE_HPP

#include <cstdint>

class PieceType {
public:
    enum Value : std::uint8_t {
        PAWN,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING
    };

    PieceType() = default;

    PieceType(Value pieceType) : pieceType(pieceType) {

    }

    uint8_t getValue() {
        return pieceType;
    }

private:
    Value pieceType;

};

#endif
