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

    Piece(Piece::Value piece) : piece(piece) {

    }

    Piece(PieceType type, Color color) {
        if(color == Color::NONE) piece = Piece::NONE;
        if(type.getValue() == PieceType::NONE) piece = Piece::NONE;
        piece = static_cast<Value>(color.getValue() * 6 + type.getIndex());
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

    [[nodiscard]] char getCharacter() const {
        switch(piece) {
            case Value::WHITE_PAWN:
                return 'P';
            case Value::WHITE_KNIGHT:
                return 'N';
            case Value::WHITE_BISHOP:
                return 'B';
            case Value::WHITE_ROOK:
                return 'R';
            case Value::WHITE_QUEEN:
                return 'Q';
            case Value::WHITE_KING:
                return 'K';
            case Value::BLACK_PAWN:
                return 'p';
            case Value::BLACK_KNIGHT:
                return 'n';
            case Value::BLACK_BISHOP:
                return 'b';
            case Value::BLACK_ROOK:
                return 'r';
            case Value::BLACK_QUEEN:
                return 'q';
            case Value::BLACK_KING:
                return 'k';
            case Value::NONE:
                return ' ';
            default:
                return 0;
        }
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
