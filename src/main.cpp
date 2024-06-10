#include "board.hpp"
#include "attacks.hpp"
#include "movegen.hpp"
#include "perft.hpp"

int main() {
    Attacks::initMagics();
    MoveGen::initSquaresBetween();
    return 0;
}