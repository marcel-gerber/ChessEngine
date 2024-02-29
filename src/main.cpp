#include <iostream>
#include "color.hpp"
#include "board.hpp"

int main() {
    Board board = Board();
    board.getCastlingRights()->set(Castling::BLACK_000);
    board.getCastlingRights()->set(Castling::WHITE_00);
    board.getCastlingRights()->unset(Castling::WHITE_00);

    std::cout << board.getCastlingRights()->has(Castling::WHITE_00) << std::endl;
    std::cout << +board.getCastlingRights()->getCastlingRights() << std::endl;

    auto square = Square(Square::NONE);
    std::cout << +square.getIndex();

    return 0;
}
