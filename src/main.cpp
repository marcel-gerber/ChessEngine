#include "attacks.hpp"
#include "movegen.hpp"
#include "uci.hpp"

int main() {
    Attacks::initMagics();
    MoveGen::initSquaresBetween();

    UCIHandler uciHandler;
    uciHandler.inputLoop();

    return 0;
}