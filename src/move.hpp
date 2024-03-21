//
// Created by Marcel on 21.03.2024.
//

#ifndef CHESSENGINE_MOVE_HPP
#define CHESSENGINE_MOVE_HPP

#include <cstdint>
#include "piece.hpp"

class MoveType {
public:
    enum class Value : uint16_t {
        NORMAL,
        PROMOTION = 1 << 14,
        EN_PASSANT = 2 << 14,
        CASTLING = 3 << 14
    };

    static constexpr uint16_t NORMAL = static_cast<uint16_t>(Value::NORMAL);
    static constexpr uint16_t PROMOTION = static_cast<uint16_t>(Value::PROMOTION);
    static constexpr uint16_t EN_PASSANT = static_cast<uint16_t>(Value::EN_PASSANT);
    static constexpr uint16_t CASTLING = static_cast<uint16_t>(Value::CASTLING);
};

class Move {
public:
    Move() = default;

    constexpr explicit Move(uint16_t move) : move(move) {

    }

    template<uint16_t moveType>
    static constexpr Move create(uint8_t from, uint8_t to, PieceType pieceType = PieceType::KNIGHT) {
        return Move(moveType + ((pieceType - PieceType::KNIGHT) << 12) + (from << 6) + to);
    }

    [[nodiscard]] constexpr uint8_t from_index() const {
        return ((move >> 6) & 0x3F);
    }

    [[nodiscard]] constexpr uint8_t to_index() const {
        return (move & 0x3F);
    }

    // Get the MoveType
    [[nodiscard]] constexpr uint16_t type() const {
        return (move & (3 << 14));
    }

    // Get the PieceType of the promotion
    [[nodiscard]] constexpr uint16_t promotion_type() const {
        return (((move >> 12) & 3) + static_cast<uint8_t>(PieceType::KNIGHT));
    }

private:
    uint16_t move;
};

#endif