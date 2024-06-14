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
    static void set(uint64_t &bits, const uint8_t &index);

    static void unset(uint64_t &bits, const uint8_t &index);

    static bool isSet(const uint64_t &bits, const uint8_t &index);

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

    static std::vector<uint8_t> getIndices(const uint64_t &bits);

    static void print(const uint64_t &bits);
};

#endif
