//
// Created by Marcel on 08.03.2024.
//

#ifndef CHESSENGINE_BITS_HPP
#define CHESSENGINE_BITS_HPP

#include <cstdint>
#include <cassert>
#include <bit>
#include <vector>
#include <iostream>

class Bits {
public:
    static void set(uint64_t &bits, const uint8_t &index) {
        bits |= (1ULL << index);
    }

    static void unset(uint64_t &bits, const uint8_t &index) {
        bits &= ~(1ULL << index);
    }

    static bool isSet(const uint64_t &bits, const uint8_t &index) {
        return bits & (1ULL << index);
    }

    static constexpr uint8_t popcount(uint64_t &bits) {
        return std::popcount(bits);
    }

    static constexpr uint8_t popcount(uint64_t &&bits) {
        return std::popcount(bits);
    }

    static constexpr uint8_t lsb(uint64_t &bits) {
        return std::countr_zero(bits);
    }

    static constexpr uint8_t pop(uint64_t &bits) {
        uint8_t index = std::countr_zero(bits);
        bits &= bits - 1;
        return index;
    }

    static std::vector<uint8_t> getIndices(const uint64_t &bits) {
        std::vector<uint8_t> indices;

        for(uint8_t i = 0; i < 64; i++) {
            if(((bits >> i) & 1) == 1) {
                indices.push_back(i);
            }
        }
        return indices;
    }

    static void print(const uint64_t &bits) {
        int index = 56;

        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                if(((bits >> index) & 1) == 1) {
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
