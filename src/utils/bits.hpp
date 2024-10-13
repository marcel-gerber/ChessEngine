#ifndef CHESSENGINE_BITS_HPP
#define CHESSENGINE_BITS_HPP

#include <bit>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

class Bits {

public:
    /// Sets the bit at 'index'
    static void set(uint64_t &bits, const uint8_t &index) {
        bits |= (1ULL << index);
    }

    /// Unsets the bit at 'index'
    static void unset(uint64_t &bits, const uint8_t &index) {
        bits &= ~(1ULL << index);
    }

    /// Returns 'true' if the bit at 'index' is set
    static bool isSet(const uint64_t &bits, const uint8_t &index) {
        return bits & (1ULL << index);
    }

    /// Returns the amount of set bits
    static constexpr uint8_t popcount(uint64_t &bits) {
        return std::popcount(bits);
    }

    /// Returns the amount of set bits
    static constexpr uint8_t popcount(uint64_t &&bits) {
        return std::popcount(bits);
    }

    /// Returns the index of the least significant bit
    static constexpr uint8_t lsb(uint64_t &bits) {
        return std::countr_zero(bits);
    }

    /// Unsets the least significant bit and returns its' index
    static constexpr uint8_t pop(uint64_t &bits) {
        uint8_t index = std::countr_zero(bits);
        bits &= bits - 1;
        return index;
    }

    /// Returns a vector containing the indices of all set bits
    static std::vector<uint8_t> getIndices(const uint64_t &bits) {
        std::vector<uint8_t> indices;

        for(uint8_t i = 0; i < 64; i++) {
            if(isSet(bits, i)) {
                indices.push_back(i);
            }
        }
        return indices;
    }

    /// Prints a Bitboard
    static void print(const uint64_t &bits) {
        int index = 56;

        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                if(isSet(bits, index)) {
                    std::cout << "1 ";
                } else {
                    std::cout << ". ";
                }
                index++;
            }
            index -= 16;
            std::cout << std::endl;
        }
    }
};

#endif
