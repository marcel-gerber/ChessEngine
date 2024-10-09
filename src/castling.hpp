#ifndef CHESSENGINE_CASTLING_HPP
#define CHESSENGINE_CASTLING_HPP

#include "color.hpp"
#include <cstdint>
#include <array>

class Castling {

private:
    // The castling rights of a board position will be stored in this 8-bit unsigned integer
    uint8_t castling_rights;

public:
    // All castling rights can be stored in just 4 bits.
    // 00: short castling (kings' side)
    // 000: long castling (queens' side)
    enum class Value : uint8_t {
        NO_CASTLING = 0,
        WHITE_00 = 0b00000001,
        WHITE_000 = 0b00000010,
        BLACK_00 = 0b00000100,
        BLACK_000 = 0b00001000
    };

    Castling();

    /// Returns "raw" numeric value of castling rights. 0 <= raw <= 15
    [[nodiscard]] uint8_t raw() const;

    /// Sets a castling right
    void set(const Value &castling);

    /// Unsets a castling right
    void unset(const Value &castling);

    /// Unsets both castling rights (short and long) for a side ('Color')
    void unset(const Color &color);

    /// Resets the castling rights to 0 (NO_CASTLING)
    void reset();

    /// Returns 'true' when the castling right 'Value' is set
    [[nodiscard]] bool has(const Value &castling) const;

    /// Checks whether the side ('Color') has a castling right (short or long)
    [[nodiscard]] bool has(const Color &color) const;

    /// Returns 'true' if neither side has castling rights
    [[nodiscard]] bool hasNoCastling() const;

    /// Returns the two castling rights based off the 'Color'
    template<Color::Value color>
    static constexpr std::array<Value, 2> getCastlings() {
        switch(color) {
            case Color::WHITE:
                return {WHITE_00, WHITE_000};
            case Color::BLACK:
                return {BLACK_00, BLACK_000};
            default:
                return { };
        }
    }

    /// Returns the kings' target square index based off the castling right 'Value'
    static constexpr uint8_t kingTargetIndex(const Value &castling) {
        switch(castling) {
            case WHITE_00:
                return 6;
            case WHITE_000:
                return 2;
            case BLACK_00:
                return 62;
            case BLACK_000:
                return 58;
            default:
                return -1;
        }
    }

    /// Returns the rooks' target square index based off the castling right 'Value'
    static constexpr uint8_t rookTargetIndex(const Value &castling) {
        switch(castling) {
            case WHITE_00:
                return 5;
            case WHITE_000:
                return 3;
            case BLACK_00:
                return 61;
            case BLACK_000:
                return 59;
            default:
                return -1;
        }
    }

    /// Returns the rooks' source square index based off the castling right 'Value'
    static constexpr uint8_t rookSourceIndex(const Value &castling) {
        switch(castling) {
            case WHITE_00:
                return 7;
            case WHITE_000:
                return 0;
            case BLACK_00:
                return 63;
            case BLACK_000:
                return 56;
            default:
                return -1;
        }
    }

    /// Returns the castling right 'Value' based on the rooks' source square index
    static constexpr Value fromRookSourceIndex(const uint8_t &index) {
        switch(index) {
            case 0:
                return WHITE_000;
            case 7:
                return WHITE_00;
            case 56:
                return BLACK_000;
            case 63:
                return BLACK_00;
            default:
                return NO_CASTLING;
        }
    }

    /// Returns the castling right 'Value' based on the kings' target square index
    static constexpr Value fromKingTargetIndex(const uint8_t &index) {
        switch(index) {
            case 2:
                return WHITE_000;
            case 6:
                return WHITE_00;
            case 58:
                return BLACK_000;
            case 62:
                return BLACK_00;
            default:
                return NO_CASTLING;
        }
    }

    static constexpr Value NO_CASTLING = Value::NO_CASTLING;
    static constexpr Value WHITE_00 = Value::WHITE_00;
    static constexpr Value WHITE_000 = Value::WHITE_000;
    static constexpr Value BLACK_00 = Value::BLACK_00;
    static constexpr Value BLACK_000 = Value::BLACK_000;

};

#endif
