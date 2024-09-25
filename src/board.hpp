//
// Created by Marcel on 28.02.2024.
//

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
    Castling castling_rights;
    Square en_passant_square = Square::NONE;
    uint8_t half_move_clock = 0;
    Color side_to_move = Color::WHITE;

    std::stack<StateInfo> prev_state_infos;

    // Used to store the zobrist hash and how often this position has occurred
    std::unordered_map<uint64_t, uint8_t> repetition_table;

    void incrementRepetition(const uint64_t &zobrist_key);

    void decrementRepetition(const uint64_t &zobrist_key);

    [[nodiscard]] uint64_t calculateZobrist() const;

public:
    Board();

    [[nodiscard]] uint64_t getZobrist() const;

    [[nodiscard]] Castling getCastlingRights() const;

    [[nodiscard]] const Square* getEnPassantSquare() const;

    void setEnPassantSquare(Square square);

    [[nodiscard]] Color getSideToMove() const;

    void setSideToMove(Color color);

    [[nodiscard]] uint8_t getHalfMoveClock() const;

    void setHalfMoveClock(const uint8_t &half_move);

    void placePiece(const Piece &piece, const uint8_t &index);

    void removePiece(const Piece &piece, const uint8_t &index);

    [[nodiscard]] uint64_t getPieces(const Color &color, const PieceType &pieceType) const;

    [[nodiscard]] uint64_t getPieces(const PieceType &pieceType) const;

    [[nodiscard]] Piece getPiece(const uint8_t &index) const;

    [[nodiscard]] uint64_t getSide(Color color) const;

    [[nodiscard]] uint64_t getOccupancy() const;
    
    [[nodiscard]] uint8_t getKingIndex(Color color) const;

    void makeMove(const Move &move);

    void unmakeMove(const Move &move);

    void setFen(const std::string &fen);

    [[nodiscard]] bool isRepetition() const;

    [[nodiscard]] bool isCheck() const;

    GameResult checkForDraw(const bool &is_check) const;

    void reset();

    void print() const;

};

#endif
