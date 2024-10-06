//
// Created by Marcel on 29.02.2024.
//

#ifndef CHESSENGINE_GRID_HPP
#define CHESSENGINE_GRID_HPP

#include "color.hpp"
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

    static uint64_t bitboard(const uint8_t &index) {
        switch(static_cast<Value>(index)) {
            case Value::FILE_A:
                return FILE_ABB;
            case Value::FILE_B:
                return FILE_BBB;
            case Value::FILE_C:
                return FILE_CBB;
            case Value::FILE_D:
                return FILE_DBB;
            case Value::FILE_E:
                return FILE_EBB;
            case Value::FILE_F:
                return FILE_FBB;
            case Value::FILE_G:
                return FILE_GBB;
            case Value::FILE_H:
                return FILE_HBB;
            default:
                return 0ULL;
        }
    }

    static constexpr uint64_t FILE_ABB = 0x0101010101010101ULL;
    static constexpr uint64_t FILE_BBB = FILE_ABB << 1;
    static constexpr uint64_t FILE_CBB = FILE_ABB << 2;
    static constexpr uint64_t FILE_DBB = FILE_ABB << 3;
    static constexpr uint64_t FILE_EBB = FILE_ABB << 4;
    static constexpr uint64_t FILE_FBB = FILE_ABB << 5;
    static constexpr uint64_t FILE_GBB = FILE_ABB << 6;
    static constexpr uint64_t FILE_HBB = FILE_ABB << 7;

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

    static uint64_t bitboard(const uint8_t &index) {
        switch(static_cast<Value>(index)) {
            case Value::RANK_1:
                return RANK_1BB;
            case Value::RANK_2:
                return RANK_2BB;
            case Value::RANK_3:
                return RANK_3BB;
            case Value::RANK_4:
                return RANK_4BB;
            case Value::RANK_5:
                return RANK_5BB;
            case Value::RANK_6:
                return RANK_6BB;
            case Value::RANK_7:
                return RANK_7BB;
            case Value::RANK_8:
                return RANK_8BB;
            default:
                return 0ULL;
        }
    }

    template<Color::Value color>
    static constexpr uint64_t promotion() {
        switch(color) {
            case Color::WHITE:
                return RANK_8BB;
            case Color::BLACK:
                return RANK_1BB;
            default:
                return 0ULL;
        }
    }

    template<Color::Value color>
    static constexpr uint64_t doublePush() {
        switch(color) {
            case Color::WHITE:
                return RANK_3BB;
            case Color::BLACK:
                return RANK_6BB;
            default:
                return 0ULL;
        }
    }

    static constexpr uint64_t RANK_1BB = 0xFF;
    static constexpr uint64_t RANK_2BB = RANK_1BB << (8 * 1);
    static constexpr uint64_t RANK_3BB = RANK_1BB << (8 * 2);
    static constexpr uint64_t RANK_4BB = RANK_1BB << (8 * 3);
    static constexpr uint64_t RANK_5BB = RANK_1BB << (8 * 4);
    static constexpr uint64_t RANK_6BB = RANK_1BB << (8 * 5);
    static constexpr uint64_t RANK_7BB = RANK_1BB << (8 * 6);
    static constexpr uint64_t RANK_8BB = RANK_1BB << (8 * 7);

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
        if(index >= 0 && index <= 63) {
            square = static_cast<Square::Value>(index);
        }
    }

    explicit Square(const std::string &string) : square(NONE) {
        if(string == "-") {
            return;
        }

        assert(string.size() == 2);
        square = static_cast<Square::Value>((string[0] - 'a') + ((string[1] - '1') * 8));
    }

    constexpr bool operator<(const uint64_t &rhs) const {
        return index() < rhs;
    }

    constexpr void operator++(int) {
        square = static_cast<Value>(index() + 1);
    }

    [[nodiscard]] constexpr uint8_t index() const {
        return static_cast<uint8_t>(square);
    }

    [[nodiscard]] constexpr Value value() const {
        return square;
    }

    [[nodiscard]] bool isValid() const {
        return (square >= Value::A1 && square <= Value::H8);
    }

    static std::string toString(const uint8_t &index) {
        std::string string;
        string += 'a' + (index & 7);
        string += '1' + (index >> 3);

        return string;
    }

    static uint64_t toBitboard(const uint8_t &index) {
        return (1ULL << index);
    }

    static constexpr uint8_t enPassantIndex(const uint8_t &index) {
        return index ^ 8;
    }

    // https://www.chessprogramming.org/Efficient_Generation_of_Sliding_Piece_Attacks
    [[nodiscard]] uint8_t fileIndex() const {
        return index() & 7;
    }

    [[nodiscard]] uint8_t rankIndex() const {
        return index() >> 3;
    }

    [[nodiscard]] uint8_t diagonalIndex() const {
        return (rankIndex() - fileIndex()) & 15;
    }

    [[nodiscard]] uint8_t antiDiagonalIndex() const {
        return (rankIndex() + fileIndex()) ^ 7;
    }

    static constexpr Value NONE = Value::NONE;

private:
    Value square;
};

class Direction {
public:
    enum class Value : int8_t {
        NORTH = 8,
        EAST = 1,
        SOUTH = -8,
        WEST = -1,
        NORTH_EAST = 9,
        SOUTH_EAST = -7,
        SOUTH_WEST = -9,
        NORTH_WEST = 7,
        NONE = 0
    };

    static constexpr int8_t toValue(const Value &value) {
        return static_cast<int8_t>(value);
    }

    static constexpr Value NORTH = Value::NORTH;
    static constexpr Value EAST = Value::EAST;
    static constexpr Value SOUTH = Value::SOUTH;
    static constexpr Value WEST = Value::WEST;
    static constexpr Value NORTH_EAST = Value::NORTH_EAST;
    static constexpr Value SOUTH_EAST = Value::SOUTH_EAST;
    static constexpr Value SOUTH_WEST = Value::SOUTH_WEST;
    static constexpr Value NORTH_WEST = Value::NORTH_WEST;
    static constexpr Value NONE = Value::NONE;

};

#endif
