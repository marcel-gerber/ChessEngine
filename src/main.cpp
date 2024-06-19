#include "attacks.hpp"
#include "movegen.hpp"

int main() {
    Attacks::initMagics();
    MoveGen::initSquaresBetween();
    return 0;
}