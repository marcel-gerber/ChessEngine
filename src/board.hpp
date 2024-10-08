#ifndef CHESSENGINE_BOARD_HPP
#define CHESSENGINE_BOARD_HPP

#include "castling.hpp"
#include "color.hpp"
#include "grid.hpp"
#include "move.hpp"
#include "piece.hpp"

#include <cstdint>
#include <stack>
#include <unordered_map>

enum class GameResult : uint8_t {
    WIN,
    LOSS,
    DRAW,
    NONE
};

class Board {
private:
    // Contains all the necessary information of a 'Board' for unmaking a move.
    struct StateInfo {
        uint64_t hash;
        Castling castling_rights;
        Square en_passant;
        uint8_t half_move_clock;
        Piece captured;

        StateInfo(const uint64_t &hash, const Castling &castling, const Square &en_passant, const uint8_t &half_moves, const Piece &captured) :
            hash(hash),
            castling_rights(castling),
            en_passant(en_passant),
            half_move_clock(half_moves),
            captured(captured) {
        }
    };

    uint64_t bb_pieces[6] = { 0 };
    uint64_t bb_sides[2] = { 0 };
    Piece pieces[64] = { };

    uint64_t zobrist_hash = 0ULL;
    Castling castling_rights = {};
    Square en_passant_square = Square::NONE;
    uint8_t half_move_clock = 0;
    Color side_to_move = Color::WHITE;

    // Storing all previous 'StateInfo' objects in here
    std::stack<StateInfo> prev_state_infos;

    // Used to store the zobrist hash and how often this position has occurred
    std::unordered_map<uint64_t, uint8_t> repetition_table;

    /// Places a piece on the board
    void placePiece(const Piece &piece, const uint8_t &index);

    /// Removes a piece from the board
    void removePiece(const Piece &piece, const uint8_t &index);

    /// Increases the count of occurrences for this position (zobrist_key) by 1
    void incrementRepetition(const uint64_t &zobrist_key);

    /// Decreases the count of occurrences for this position (zobrist_key) by 1.
    /// The entry will be deleted if it's value is 0
    void decrementRepetition(const uint64_t &zobrist_key);

    /// Calculates the the Zobrist hash of the board. Should only be used to initialize 'zobrist_hash'
    [[nodiscard]] uint64_t calculateZobrist() const;

public:
    Board();

    /// Returns the bitboard of this 'Color' and 'PieceType'
    [[nodiscard]] uint64_t getPieces(const Color &color, const PieceType &pieceType) const;

    /// Returns the bitboard of this 'PieceType'
    [[nodiscard]] uint64_t getPieces(const PieceType &pieceType) const;

    /// Returns a 'Piece' object at this index. Can be 'NONE'
    [[nodiscard]] Piece getPiece(const uint8_t &index) const;

    /// Returns the bitboard of this 'Color'
    [[nodiscard]] uint64_t getSide(Color color) const;

    /// Returns the bitboard with all pieces currently on the board
    [[nodiscard]] uint64_t getOccupancy() const;

    /// Returns the index of the king of this 'Color' on the board
    [[nodiscard]] uint8_t getKingIndex(Color color) const;

    /// Plays a 'Move' on the board
    void makeMove(const Move &move);

    /// Undo a 'Move' on the board
    void unmakeMove(const Move &move);

    /// Sets the FEN-String of this board
    void setFen(const std::string &fen);

    /// Checks whether the current position is a repetition
    [[nodiscard]] bool isRepetition() const;

    /// Checks whether the king of the side to move is attacked by the enemy
    [[nodiscard]] bool isCheck() const;

    /// Checks whether the current position is a draw. Expects the result of 'isCheck()' as parameter to save time.
    /// Returns a 'GameResult' which can be 'DRAW', 'LOSE' or 'NONE'
    GameResult checkForDraw(const bool &is_check) const;

    /// Resets the board
    void reset();

    /// Returns the Zobrist hash of the current position
    [[nodiscard]] uint64_t getZobrist() const;

    /// Returns the current castling rights
    [[nodiscard]] Castling getCastlingRights() const;

    /// Returns a pointer to the current En Passant Square. Can be 'NONE'
    [[nodiscard]] const Square* getEnPassantSquare() const;

    /// Returns the current side to move
    [[nodiscard]] Color getSideToMove() const;

    /// Returns the current half move clock
    [[nodiscard]] uint8_t getHalfMoveClock() const;

    /// Prints the current board
    void print() const;

};

#endif
