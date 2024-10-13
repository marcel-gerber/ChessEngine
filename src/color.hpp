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

    /// Returns the opposite of the color
    [[nodiscard]] constexpr Value opposite() const {
        return (color == Value::WHITE) ? Value::BLACK : Value::WHITE;
    }

    /// Returns the opposite of the color
    template<Color::Value color>
    static constexpr Value opposite() {
        return (color == Value::WHITE) ? Value::BLACK : Value::WHITE;
    }

    /// Returns the numeric value (the index of the enum) of the enum
    [[nodiscard]] constexpr uint8_t value() const {
        return static_cast<uint8_t>(color);
    }

    /// Returns 'true' when both colors are the same
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
