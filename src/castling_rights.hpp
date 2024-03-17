//
// Created by Marcel on 29.02.2024.
//

#ifndef CHESSENGINE_CASTLING_RIGHTS_HPP
#define CHESSENGINE_CASTLING_RIGHTS_HPP

#include <cstdint>
#include <vector>

class Castling {
public:
    enum class Value : uint8_t {
        NO_CASTLING = 0,
        WHITE_00 = 0b00000001,
        WHITE_000 = 0b00000010,
        BLACK_00 = 0b00000100,
        BLACK_000 = 0b00001000
    };

    Castling() {
        castling_rights =  static_cast<uint8_t>(NO_CASTLING);
    }

    [[nodiscard]] uint8_t getCastlingRights() const {
        return castling_rights;
    }

    void set(const Value &castling) {
        castling_rights |= static_cast<uint8_t>(castling);
    }

    void unset(const Value &castling) {
        castling_rights &= ~static_cast<uint8_t>(castling);
    }

    [[nodiscard]] bool has(const Value &castling) const {
        const auto value = static_cast<uint8_t>(castling);
        return (castling_rights & value) == value;
    }

    [[nodiscard]] bool hasNoCastling() const {
        return castling_rights == static_cast<uint8_t>(NO_CASTLING);
    }

    static constexpr std::vector<Value> getCastlings(const Color &color) {
        switch(color.getValue()) {
            case static_cast<uint8_t>(Color::WHITE):
                return {WHITE_00, WHITE_000};
            case static_cast<uint8_t>(Color::BLACK):
                return {BLACK_00, BLACK_000};
            default:
                return { };
        }
    }

    static constexpr uint8_t getEndingKingIndex(const Value &castling) {
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

    static constexpr uint8_t getEndingRookIndex(const Value &castling) {
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

    static constexpr uint8_t getStartingRookIndex(const Value &castling) {
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

    static constexpr Value NO_CASTLING = Value::NO_CASTLING;
    static constexpr Value WHITE_00 = Value::WHITE_00;
    static constexpr Value WHITE_000 = Value::WHITE_000;
    static constexpr Value BLACK_00 = Value::BLACK_00;
    static constexpr Value BLACK_000 = Value::BLACK_000;

private:
    uint8_t castling_rights;
};

#endif
