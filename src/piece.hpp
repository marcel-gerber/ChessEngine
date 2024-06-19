//
// Created by Marcel on 28.02.2024.
//

#ifndef CHESSENGINE_PIECE_HPP
#define CHESSENGINE_PIECE_HPP

#include "color.hpp"

#include <cstdint>

class PieceType {

public:
    enum class Value : uint8_t {
        PAWN,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING,
        NONE
    };

    PieceType() = default;

    constexpr PieceType(Value pieceType) : pieceType(pieceType) {

    }

    constexpr explicit PieceType(const uint8_t &type) {
        pieceType = static_cast<Value>(type);
    }

    [[nodiscard]] constexpr uint8_t getIndex() const {
        return static_cast<uint8_t>(pieceType);
    }

    [[nodiscard]] constexpr Value getValue() const {
        return pieceType;
    }

    // used for Move::create
    constexpr uint16_t operator-(const PieceType &pt) const {
        return getIndex() - pt.getIndex();
    }

    static constexpr Value PAWN = Value::PAWN;
    static constexpr Value KNIGHT = Value::KNIGHT;
    static constexpr Value BISHOP = Value::BISHOP;
    static constexpr Value ROOK = Value::ROOK;
    static constexpr Value QUEEN = Value::QUEEN;
    static constexpr Value KING = Value::KING;
    static constexpr Value NONE = Value::NONE;

private:
    Value pieceType;
};

class Piece {

public:
    enum class Value : uint8_t {
        WHITE_PAWN,
        WHITE_KNIGHT,
        WHITE_BISHOP,
        WHITE_ROOK,
        WHITE_QUEEN,
        WHITE_KING,
        BLACK_PAWN,
        BLACK_KNIGHT,
        BLACK_BISHOP,
        BLACK_ROOK,
        BLACK_QUEEN,
        BLACK_KING,
        NONE
    };

    Piece() = default;

    Piece(Piece::Value piece);

    Piece(const char &c);

    Piece(PieceType type, Color color);

    bool operator!=(const Piece &p) const;

    [[nodiscard]] Color getColor() const;

    [[nodiscard]] char getCharacter() const;

    [[nodiscard]] PieceType getType() const;

    [[nodiscard]] uint8_t getValue() const;

    static constexpr Value WHITE_PAWN = Value::WHITE_PAWN;
    static constexpr Value WHITE_KNIGHT = Value::WHITE_KNIGHT;
    static constexpr Value WHITE_BISHOP = Value::WHITE_BISHOP;
    static constexpr Value WHITE_ROOK = Value::WHITE_ROOK;
    static constexpr Value WHITE_QUEEN = Value::WHITE_QUEEN;
    static constexpr Value WHITE_KING = Value::WHITE_KING;
    static constexpr Value BLACK_PAWN = Value::BLACK_PAWN;
    static constexpr Value BLACK_KNIGHT = Value::BLACK_KNIGHT;
    static constexpr Value BLACK_BISHOP = Value::BLACK_BISHOP;
    static constexpr Value BLACK_ROOK = Value::BLACK_ROOK;
    static constexpr Value BLACK_QUEEN = Value::BLACK_QUEEN;
    static constexpr Value BLACK_KING = Value::BLACK_KING;
    static constexpr Value NONE = Value::NONE;

private:
    Value piece;
};

#endif
