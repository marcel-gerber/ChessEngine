#include "bits.hpp"

void Bits::set(uint64_t &bits, const uint8_t &index) {
    bits |= (1ULL << index);
}

void Bits::unset(uint64_t &bits, const uint8_t &index) {
    bits &= ~(1ULL << index);
}

bool Bits::isSet(const uint64_t &bits, const uint8_t &index) {
    return bits & (1ULL << index);
}

std::vector<uint8_t> Bits::getIndices(const uint64_t &bits) {
    std::vector<uint8_t> indices;

    for(uint8_t i = 0; i < 64; i++) {
        if(((bits >> i) & 1) == 1) {
            indices.push_back(i);
        }
    }
    return indices;
}

void Bits::print(const uint64_t &bits) {
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