#ifndef CHESSENGINE_RANDOMGEN_HPP
#define CHESSENGINE_RANDOMGEN_HPP

#include <iomanip>
#include <iostream>
#include <random>
#include <string>

// class for generating random 64-bit integers
// for Zobrist hashing
class RandomNumberGen {

private:
    /// Generates an 'amount' of random 64-bit integers.
    /// Prints out copyable array to the console
    static void generate(const std::string &name, const int &amount) {
        std::random_device random;
        std::mt19937_64 engine(random());

        std::uniform_int_distribution<uint64_t> distribution;

        std::cout << "static constexpr uint64_t " << name << "[" << amount << "] = {" << std::endl;
        for(int i = 0; i < amount; i++) {
            uint64_t random_number = distribution(engine);

            if(i % 4 == 0) {
                std::cout << "        ";
            }

            std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0') << random_number << "ULL, ";

            if(i % 4 == 3) {
                std::cout << std::endl;
            }
        }
        std::cout << "};" << std::endl << std::endl;
    }

public:
    /// Generates Array with random 64-bit integers for the Pieces
    static void generatePieces() { generate("Pieces", 768); }

    /// Generates Array with random 64-bit integers for en passant
    static void generateEnPassant() { generate("EnPassant", 64); }

    /// Generates Array with random 64-bit integers for Castling
    static void generateCastling() { generate("Castling", 16); }

    /// Generates Array with random 64-bit integers for the side to move
    static void generateSideToMove() { generate("SideToMove", 1); }
};

#endif
