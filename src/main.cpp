#include "attacks.hpp"
#include "movegen.hpp"
#include "uci.hpp"

int main() {
    Attacks::initMagics();
    MoveGen::initSquaresBetween();

    UCIHandler uciHandler;
    std::string input;

    while(std::getline(std::cin, input)) {
        if(input == "quit") break;

        uciHandler.handleCommand(input);
    }

    return 0;
}