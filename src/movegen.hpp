#ifndef CHESSENGINE_MOVEGEN_HPP
#define CHESSENGINE_MOVEGEN_HPP

#include "utils/bits.hpp"
#include "board.hpp"
#include "color.hpp"
#include "move.hpp"

#include <cstdint>
#include <tuple>
#include <vector>

enum class MoveGenType : uint8_t {
    ALL,
    CAPTURE,
    QUIET
};

class MoveGen {

private:
    // The squares between two squares will be saved in here as a bitboard.
    static inline uint64_t SQUARES_BETWEEN[64][64] = { 0 };

    // Logic taken from Disservins' "chess-library"
    // Source: https://github.com/Disservin/chess-library/blob/a4404d5c781183fab09b0d76eac0df4b3a6cdf93/src/movegen.hpp#L476
    template<typename T>
    static void addMoveToMoveList(std::vector<Move> &moves, uint64_t &bb_from, T function) {
        while(bb_from) {
            const uint8_t index_from = Bits::pop(bb_from);
            uint64_t bb_moves = function(index_from);

            while(bb_moves) {
                const uint8_t index_to = Bits::pop(bb_moves);
                moves.push_back(Move::create<MoveType::NORMAL>(index_from, index_to));
            }
        }
    }

    /// Shifts a bitboard either up or down one rank
    static constexpr uint64_t shift(uint64_t bits, const int8_t &direction) {
        switch(direction) {
            case 8:
                return bits << 8;
            case -8:
                return bits >> 8;
            default:
                return 0ULL;
        }
    }

    /// Adds all promotion moves to 'moves' given a squares' target index and a direction
    static void promotionMoves(std::vector<Move> &moves, const uint8_t &target_index, const int8_t &direction);

public:
    /// Initializes the squares-between array. Should only be called once at startup
    static void initSquaresBetween();

    /// Returns a bitboard containing the pin mask for the horizontal and vertical attack rays
    template<Color::Value color>
    static uint64_t pinMaskHV(const Board &board);

    /// Returns a bitboard containing the pin mask for the diagonal attack rays
    template<Color::Value color>
    static uint64_t pinMaskDiagonal(const Board &board);

    /// Returns a bitboard containing all the opponents' attacked squares
    template<Color::Value color>
    static uint64_t attackedSquares(const Board &board);

    /// Returns a bitboard containing the check mask
    template<Color::Value color>
    static std::tuple<uint64_t, uint8_t> checkMask(const Board &board);

    /// Generate all pawns' legal moves based on the 'Color' and 'MoveType'
    template<Color::Value color, MoveGenType moveGenType>
    static void generatePawnMoves(const Board &board, std::vector<Move> &moves, const uint64_t &pin_hv,
                                  const uint64_t &pin_d, const uint64_t &checkmask);

    /// Returns a bitboard containing all legal moves for a knight standing on the squares' index
    static uint64_t generateKnightMoves(const uint8_t &index);

    /// Returns a bitboard containing all legal moves for a bishop standing on the squares' index
    static uint64_t generateBishopMoves(const uint8_t &index, const uint64_t &pin_d, const uint64_t &bb_occupied);

    /// Returns a bitboard containing all legal moves for a rook standing on the squares' index
    static uint64_t generateRookMoves(const uint8_t &index, const uint64_t &pin_hv, const uint64_t &bb_occupied);

    /// Returns a bitboard containing all legal moves for a queen standing on the squares' index
    static uint64_t generateQueenMoves(const uint8_t &index, const uint64_t &pin_hv, const uint64_t &pin_d,
                                       const uint64_t &bb_occupied);

    /// Returns a bitboard containing all legal moves for a king standing on the squares' index
    static uint64_t generateKingMoves(const uint8_t &index, const uint64_t &bb_attacked,
                                      const uint64_t &bb_movable_squares);

    /// Adds all legal castling moves to the movelist
    template<Color::Value color, MoveGenType moveGenType>
    static void generateCastleMoves(const Board &board, std::vector<Move> &moves, const uint64_t &bb_attacked);

    /// Adds all legal moves of a current position to the movelist given a 'Color' and 'MoveType'
    template<Color::Value color, MoveGenType moveGenType>
    static void legalMoves(const Board &board, std::vector<Move> &moves);

    /// Adds all legal moves of a current position to the movelist given a 'MoveType'
    template<MoveGenType moveGenType>
    static void legalMoves(const Board &board, std::vector<Move> &moves);

};

#endif
