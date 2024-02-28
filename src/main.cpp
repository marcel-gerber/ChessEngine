#include <iostream>
#include "color.hpp"
#include "board.hpp"

int main() {
    Color color = Color::BLACK;

    std::cout << +color.getOppositeColor() << std::endl;
    return 0;
}
