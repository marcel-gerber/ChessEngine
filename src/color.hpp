//
// Created by Marcel on 28.02.2024.
//

#ifndef CHESSENGINE_COLOR_HPP
#define CHESSENGINE_COLOR_HPP

#include <cstdint>

class Color {
public:
    enum class Value : uint8_t {
        WHITE,
        BLACK,
        NONE
    };

    Color() = default;

    Color(Value color) : color(color) {

    }

    Value getOppositeColor() {
        return (color == Value::WHITE) ? Value::BLACK : Value::WHITE;
    }

    [[nodiscard]] uint8_t getValue() const {
        return static_cast<uint8_t>(color);
    }

    static constexpr Value WHITE = Value::WHITE;
    static constexpr Value BLACK = Value::BLACK;
    static constexpr Value NONE = Value::NONE;

private:
    Value color;
};

#endif
