//
// Created by Marcel on 28.02.2024.
//

#ifndef CHESSENGINE_PIECE_HPP
#define CHESSENGINE_PIECE_HPP

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

    PieceType(Value pieceType) : pieceType(pieceType) {

    }

    [[nodiscard]] uint8_t getValue() const {
        return static_cast<uint8_t>(pieceType);
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

    Piece(Piece::Value piece) : piece(piece) {

    }

    Piece(const char &c) : piece(Value::NONE) {
        switch(c) {
            case 'P':
                piece = WHITE_PAWN;
                break;
            case 'N':
                piece = WHITE_KNIGHT;
                break;
            case 'B':
                piece = WHITE_BISHOP;
                break;
            case 'R':
                piece = WHITE_ROOK;
                break;
            case 'Q':
                piece = WHITE_QUEEN;
                break;
            case 'K':
                piece = WHITE_KING;
                break;
            case 'p':
                piece = BLACK_PAWN;
                break;
            case 'n':
                piece = BLACK_KNIGHT;
                break;
            case 'b':
                piece = BLACK_BISHOP;
                break;
            case 'r':
                piece = BLACK_ROOK;
                break;
            case 'q':
                piece = BLACK_QUEEN;
                break;
            case 'k':
                piece = BLACK_KING;
                break;
            default:
                piece = NONE;
                break;
        }
    }

    bool operator!=(const Piece &p) const {
        return piece != p.piece;
    }

    [[nodiscard]] Color getColor() const {
        if(piece >= WHITE_PAWN && piece <= WHITE_KING) {
            return Color::WHITE;
        }

        if(piece >= BLACK_PAWN && piece <= BLACK_KING) {
            return Color::BLACK;
        }
        return Color::NONE;
    }

    [[nodiscard]] PieceType getType() const {
        return static_cast<PieceType::Value>((uint8_t) piece % 6);
    }

    [[nodiscard]] uint8_t getValue() const {
        return static_cast<uint8_t>(piece);
    }

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
