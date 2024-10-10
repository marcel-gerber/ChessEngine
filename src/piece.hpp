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

    constexpr PieceType(Value pieceType) : pieceType(pieceType) { }

    /// Constructor for creating a 'PieceType' with its' index
    constexpr explicit PieceType(const uint8_t &type) {
        pieceType = static_cast<Value>(type);
    }

    /// Returns the enums' index of the 'PieceType'
    [[nodiscard]] constexpr uint8_t index() const {
        return static_cast<uint8_t>(pieceType);
    }

    /// Returns the 'PieceTypes' enum
    [[nodiscard]] constexpr Value value() const {
        return pieceType;
    }

    /// Operator used for Move::create()
    constexpr uint16_t operator-(const PieceType &pt) const {
        return index() - pt.index();
    }

    /// Returns the 'PieceTypes' character
    [[nodiscard]] constexpr char character() const {
        switch(pieceType) {
            case Value::PAWN:
                return 'p';
            case Value::KNIGHT:
                return 'n';
            case Value::BISHOP:
                return 'b';
            case Value::ROOK:
                return 'r';
            case Value::QUEEN:
                return 'q';
            case Value::KING:
                return 'k';
            default:
                return ' ';
        }
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

    /// Returns 'true' when both Pieces' are not equal
    bool operator!=(const Piece &p) const;

    /// Returns the Pieces' enum index
    [[nodiscard]] constexpr uint8_t index() const {
        return static_cast<uint8_t>(piece);
    }

    /// Returns the Pieces' color
    [[nodiscard]] Color color() const;

    /// Returns the Pieces' character
    [[nodiscard]] char character() const;

    /// Returns the Pieces' type
    [[nodiscard]] PieceType type() const;

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
