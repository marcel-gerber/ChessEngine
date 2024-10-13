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

// A move can be stored in 16 bits.
//
// bit 0 - 5:   target square index (from 0 to 63)
// bit 6 - 11:  source square index (from 0 to 63)
// bit 12 - 13: promotion piece type (from knight [0] to queen [3])
// bit 14 - 15: move type (normal [0], promotion [1], en passant [2], castling [3])
//
// This logic has been copied from Stockfish
// Source: https://github.com/official-stockfish/Stockfish/blob/master/src/types.h
class Move {

public:
    Move() = default;

    constexpr explicit Move(uint16_t move) : move(move) { }

    /// Creates a move
    template<uint16_t moveType>
    static constexpr Move create(uint8_t from, uint8_t to, PieceType pieceType = PieceType::KNIGHT) {
        return Move(moveType + ((pieceType - PieceType::KNIGHT) << 12) + (from << 6) + to);
    }

    /// Returns the moves' source square index
    [[nodiscard]] constexpr uint8_t fromIndex() const {
        return ((move >> 6) & 0x3F);
    }

    /// Returns the moves' target square index
    [[nodiscard]] constexpr uint8_t toIndex() const {
        return (move & 0x3F);
    }

    /// Returns the 'MoveType' of the move as a numeric value
    [[nodiscard]] constexpr uint16_t type() const {
        return (move & (3 << 14));
    }

    /// Returns the 'PieceType' of the promotion piece as a numeric value.
    /// Should only be called when the move is a promotion move
    [[nodiscard]] constexpr uint8_t promotionType() const {
        return (((move >> 12) & 3) + static_cast<uint8_t>(PieceType::KNIGHT));
    }

    /// Returns the score of the move
    [[nodiscard]] constexpr int16_t score() const {
        return score_;
    }

    /// Sets the score of the move
    constexpr void setScore(const int16_t &score) {
        score_ = score;
    }

    /// Returns the raw numeric value of the move
    [[nodiscard]] constexpr uint16_t raw() const {
        return move;
    }

    /// Returns 'true' if both moves have the same raw numeric value
    constexpr bool operator==(const Move &other) const {
        return move == other.raw();
    }

    /// Converts the move to a UCI-string (e.g., e2e4)
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