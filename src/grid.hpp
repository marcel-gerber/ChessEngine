//
// Created by Marcel on 29.02.2024.
//

#ifndef CHESSENGINE_GRID_HPP
#define CHESSENGINE_GRID_HPP

#include <cstdint>
#include <cassert>
#include <string>

class File {
public:
    enum class Value : uint8_t {
        FILE_A,
        FILE_B,
        FILE_C,
        FILE_D,
        FILE_E,
        FILE_F,
        FILE_G,
        FILE_H
    };

    static constexpr uint64_t FILE_A = 0x0101010101010101ULL;
    static constexpr uint64_t FILE_B = FILE_A << 1;
    static constexpr uint64_t FILE_C = FILE_A << 2;
    static constexpr uint64_t FILE_D = FILE_A << 3;
    static constexpr uint64_t FILE_E = FILE_A << 4;
    static constexpr uint64_t FILE_F = FILE_A << 5;
    static constexpr uint64_t FILE_G = FILE_A << 6;
    static constexpr uint64_t FILE_H = FILE_A << 7;

};

class Rank {
public:
    enum class Value : uint8_t {
        RANK_1,
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8
    };

    static constexpr uint64_t RANK_1 = 0xFF;
    static constexpr uint64_t RANK_2 = RANK_1 << (8 * 1);
    static constexpr uint64_t RANK_3 = RANK_1 << (8 * 2);
    static constexpr uint64_t RANK_4 = RANK_1 << (8 * 3);
    static constexpr uint64_t RANK_5 = RANK_1 << (8 * 4);
    static constexpr uint64_t RANK_6 = RANK_1 << (8 * 5);
    static constexpr uint64_t RANK_7 = RANK_1 << (8 * 6);
    static constexpr uint64_t RANK_8 = RANK_1 << (8 * 7);

};

class Square {
public:
    enum class Value : uint8_t {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
        NONE
    };

    Square() = default;

    Square(Value square) : square(square) {

    }

    Square(const uint8_t &index) : square(NONE) {
        assert(index >= 0 && index <= 63);
        square = static_cast<Square::Value>(index);
    }

    Square(const std::string &string) : square(NONE) {
        if(string == "-") {
            return;
        }

        assert(string.size() == 2);
        square = static_cast<Square::Value>((string[0] - 'a') + ((string[1] - '1') * 8));
    }

    [[nodiscard]] uint8_t getIndex() const {
        return static_cast<uint8_t>(square);
    }

    // https://www.chessprogramming.org/Efficient_Generation_of_Sliding_Piece_Attacks
    [[nodiscard]] uint8_t getFileIndex() const {
        return getIndex() >> 3;
    }

    [[nodiscard]] uint8_t getRankIndex() const {
        return getIndex() & 7;
    }

    [[nodiscard]] uint8_t getDiagonalIndex() const {
        return (getRankIndex() - getFileIndex()) & 15;
    }

    [[nodiscard]] uint8_t getAntiDiagonalIndex() const {
        return (getRankIndex() + getFileIndex()) ^ 7;
    }

    static constexpr Value NONE = Value::NONE;

private:
    Value square;
};

#endif
