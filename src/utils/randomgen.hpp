#ifndef CHESSENGINE_RANDOMGEN_HPP
#define CHESSENGINE_RANDOMGEN_HPP

#include <iostream>
#include <iomanip>
#include <random>
#include <string>

// class for generating random 64-bit integers
// for Zobrist hashing
class RandomNumberGen {

private:
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
    static void generate_pieces() { generate("Pieces", 768); }

    static void generate_en_passant() { generate("EnPassant", 64); }

    static void generate_castling() { generate("Castling", 16); }

    static void generate_side_to_move() { generate("SideToMove", 1); }
};

#endif
