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

    /// Returns a bitboard of the file given its' index
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

    /// Returns a bitboard of the rank given its' index
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

    /// Returns a bitboard of the promotion rank based on the 'Color'
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

    /// Returns a bitboard of the double push rank based on the 'Color'
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

// Class for representing a square on the board.
// We will be using the "Little-Endian Rank-File Mapping" (LERF).
// More information here: https://www.chessprogramming.org/Square_Mapping_Considerations
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

    /// Creates a 'Square' object off a string. Expects exactly two characters.
    /// First: lower case character between 'a' and 'h'
    /// Second: number between 1 and 8
    explicit Square(const std::string &string) : square(NONE) {
        if(string == "-") {
            return;
        }

        assert(string.size() == 2);
        square = static_cast<Square::Value>((string[0] - 'a') + ((string[1] - '1') * 8));
    }

    /// Returns 'true' when the lefts' square index is lower than the rights' square index
    constexpr bool operator<(const uint64_t &rhs) const {
        return index() < rhs;
    }

    /// Increases the squares' index by one
    constexpr void operator++(int) {
        square = static_cast<Value>(index() + 1);
    }

    /// Returns the index of the square
    [[nodiscard]] constexpr uint8_t index() const {
        return static_cast<uint8_t>(square);
    }

    /// Returns the 'Value' of the square
    [[nodiscard]] constexpr Value value() const {
        return square;
    }

    /// Returns 'true' if the squares' index is between 0 and 63
    [[nodiscard]] bool isValid() const {
        return (square >= Value::A1 && square <= Value::H8);
    }

    /// Converts the square to a string (e.g., e4)
    static std::string toString(const uint8_t &index) {
        std::string string;
        string += 'a' + (index & 7);
        string += '1' + (index >> 3);

        return string;
    }

    /// Returns the bitboard of the square. Only the bit of the squares' index will be set
    static uint64_t toBitboard(const uint8_t &index) {
        return (1ULL << index);
    }

    /// Calculates the squares' en passant index
    static constexpr uint8_t enPassantIndex(const uint8_t &index) {
        return index ^ 8;
    }

    /// Returns the squares' file index
    [[nodiscard]] uint8_t fileIndex() const {
        return index() & 7;
    }

    /// Returns the squares' rank index
    [[nodiscard]] uint8_t rankIndex() const {
        return index() >> 3;
    }

    /// Returns the squares' diagonal index
    [[nodiscard]] uint8_t diagonalIndex() const {
        return (rankIndex() - fileIndex()) & 15;
    }

    /// Returns the squares' anti diagonal index
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

    /// Returns the enums' numeric value
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
