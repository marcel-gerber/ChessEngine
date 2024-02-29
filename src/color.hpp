//
// Created by Marcel on 28.02.2024.
//

#ifndef CHESSENGINE_COLOR_HPP
#define CHESSENGINE_COLOR_HPP

#include <cstdint>

class Color {
public:
    enum Value : uint8_t {
        WHITE,
        BLACK
    };

    Color() = default;

    Color(Value color) : color(color) {

    }

    Value getOppositeColor() {
        return (color == Value::WHITE) ? Value::BLACK : Value::WHITE;
    }

    uint8_t getValue() {
        return color;
    }

private:
    Value color;
};

#endif
