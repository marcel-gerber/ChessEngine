//
// Created by Marcel on 08.03.2024.
//

#ifndef CHESSENGINE_MISC_HPP
#define CHESSENGINE_MISC_HPP

#include <cstdint>
#include <cassert>
#include <bit>

class Misc {
public:
    static bool isSet(uint64_t &bits, const uint8_t &index) {
        return bits & (1ULL << index);
    }

    static constexpr uint8_t popcount(uint64_t &bits) {
        return std::popcount(bits);
    }
};

#endif
