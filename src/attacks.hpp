#ifndef CHESSENGINE_ATTACKS_HPP
#define CHESSENGINE_ATTACKS_HPP

#include "piece.hpp"
#include "grid.hpp"

#include <cstdint>

class Attacks {

public:
    // This object represents one entry in the Magic Table (Rook and Bishop). The so called 'fancy' approach will be
    // used. More information here: https://www.chessprogramming.org/Magic_Bitboards#Fancy
    struct Magic {
        uint64_t* attacks;
        uint64_t mask;
        uint64_t magic;
        uint8_t shift;

        /// Calculates the index needed for the 'attacks'
        [[nodiscard]] uint32_t getIndex(const uint64_t &bitboard) const {
            return ((bitboard & mask) * magic) >> shift;
        }
    };

    // Magic Tables for Rook and Bishop. It is indexed by the current position of the rook/bishop
    static inline Magic RookTable[64] = {};
    static inline Magic BishopTable[64] = {};

    // All the attacks of a rook/bishop will be saved as a bitboard in here
    static inline uint64_t RookAttacks[0x19000] = {};
    static inline uint64_t BishopAttacks[0x1480] = {};

    /// Adds an attack ray to the bitboard 'attacks_bb'. Should only be used for 'calculateRook' and 'calculateBishop'
    static void addRay(const Square &square, const Direction::Value &direction,
                       uint64_t &attacks_bb, const uint64_t &occupied);

    /// Calculates the attack of a rook standing on this 'Square' with the occupancy bitboard.
    /// Should only be used to initialize the Magic Table
    static uint64_t calculateRook(Square &square, uint64_t occupied);

    /// Calculates the attack of a bishop standing on this 'Square' with the occupancy bitboard.
    /// Should only be used to initialize the Magic Table
    static uint64_t calculateBishop(Square &square, uint64_t occupied);

    /// Initializes one entry of a Magic Table. Should only be called by 'initMagics()'
    static void initMagicEntry(PieceType &&pieceType, Square &&square, Magic table[], uint64_t magic);

public:
    /// Returns a bitboard representing the pawns' right-side attacks for this color
    static uint64_t pawnRight(const uint64_t &bb_pawns, const Color &color);

    /// Returns a bitboard representing the pawns' left-side attacks for this color
    static uint64_t pawnLeft(const uint64_t &bb_pawns, const Color &color);

    /// Returns a bitboard representing a pawns' attacks for this color at this index
    static uint64_t pawn(const Color &color, const uint8_t &index);

    /// Returns a bitboard representing a knights' attacks at this index
    static uint64_t knight(const uint8_t &index);

    /// Returns a bitboard representing a kings' attacks at this index
    static uint64_t king(const uint8_t &index);

    /// Returns a bitboard representing a rooks' attacks at this index with this occupancy bitboard
    static uint64_t rook(const uint8_t &index, const uint64_t &occupied);

    /// Returns a bitboard representing a bishops' attacks at this index with this occupancy bitboard
    static uint64_t bishop(const uint8_t &index, const uint64_t &occupied);

    /// Returns a bitboard representing a queens' attacks at this index with this occupancy bitboard
    static uint64_t queen(const uint8_t &index, const uint64_t &occupied);

    /// Initializes both Magic Tables. Should only be called once at startup
    static void initMagics();
};

#endif
