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

    Color(Value color) : color(color) { }

    [[nodiscard]] constexpr Value opposite() const {
        return (color == Value::WHITE) ? Value::BLACK : Value::WHITE;
    }

    template<Color::Value color>
    static constexpr Value opposite() {
        return (color == Value::WHITE) ? Value::BLACK : Value::WHITE;
    }

    [[nodiscard]] constexpr uint8_t value() const {
        return static_cast<uint8_t>(color);
    }

    constexpr bool operator==(const Color &rhs) const {
        return value() == rhs.value();
    }

    static constexpr Value WHITE = Value::WHITE;
    static constexpr Value BLACK = Value::BLACK;
    static constexpr Value NONE = Value::NONE;

private:
    Value color;
};

#endif
