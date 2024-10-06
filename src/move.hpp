//
// Created by Marcel on 21.03.2024.
//

#ifndef CHESSENGINE_MOVE_HPP
#define CHESSENGINE_MOVE_HPP

#include "piece.hpp"
#include "grid.hpp"

#include <cstdint>
#include <string>

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

    [[nodiscard]] constexpr uint8_t fromIndex() const {
        return ((move >> 6) & 0x3F);
    }

    [[nodiscard]] constexpr uint8_t toIndex() const {
        return (move & 0x3F);
    }

    // Get the MoveType
    [[nodiscard]] constexpr uint16_t type() const {
        return (move & (3 << 14));
    }

    // Get the PieceType of the promotion
    [[nodiscard]] constexpr uint8_t promotionType() const {
        return (((move >> 12) & 3) + static_cast<uint8_t>(PieceType::KNIGHT));
    }

    [[nodiscard]] constexpr int16_t score() const {
        return score_;
    }

    constexpr void setScore(const int16_t &score) {
        score_ = score;
    }

    [[nodiscard]] constexpr uint16_t raw() const {
        return move;
    }

    constexpr bool operator==(const Move &other) const {
        return move == other.raw();
    }

    [[nodiscard]] std::string toUCI() const {
        std::string from = Square::toString(fromIndex());
        std::string to = Square::toString(toIndex());

        if(type() == MoveType::PROMOTION) {
            auto piece_type = PieceType(promotionType());
            return {from + to + piece_type.character()};
        }
        return {from + to};
    }

private:
    uint16_t move = 0;
    int16_t score_ = 0;
};

#endif