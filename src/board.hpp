//
// Created by Marcel on 28.02.2024.
//

#ifndef CHESSENGINE_BOARD_HPP
#define CHESSENGINE_BOARD_HPP

#include <cstdint>
#include <cassert>
#include <sstream>
#include "color.hpp"
#include "piece.hpp"
#include "castling_rights.hpp"
#include "grid.hpp"
#include "misc/bits.hpp"

class Board {
private:
    uint64_t bb_pieces[6] = { 0 };
    uint64_t bb_sides[2] = { 0 };

    CastlingRights castling_rights;
    Square en_passant_square;
    uint8_t half_move_clock;
    Color side_to_move;

public:
    Board() {
        castling_rights = CastlingRights();
        en_passant_square = Square::NONE;
        side_to_move = Color::WHITE;
    }

    void placePiece(Piece &piece, uint8_t &index) {
        Bits::set(bb_pieces[piece.getType().getIndex()], index);
        Bits::set(bb_sides[piece.getColor().getValue()], index);
    }

    [[nodiscard]] uint64_t getPieces(const Color &color, const PieceType &pieceType) const {
        return bb_sides[color.getValue()] & bb_pieces[pieceType.getIndex()];
    }

    Piece getPiece(uint8_t &index) const {
        if(Bits::isSet(getPieces(Color::WHITE, PieceType::PAWN), index)) return Piece::WHITE_PAWN;
        if(Bits::isSet(getPieces(Color::WHITE, PieceType::KNIGHT), index)) return Piece::WHITE_KNIGHT;
        if(Bits::isSet(getPieces(Color::WHITE, PieceType::BISHOP), index)) return Piece::WHITE_BISHOP;
        if(Bits::isSet(getPieces(Color::WHITE, PieceType::ROOK), index)) return Piece::WHITE_ROOK;
        if(Bits::isSet(getPieces(Color::WHITE, PieceType::QUEEN), index)) return Piece::WHITE_QUEEN;
        if(Bits::isSet(getPieces(Color::WHITE, PieceType::KING), index)) return Piece::WHITE_KING;

        if(Bits::isSet(getPieces(Color::BLACK, PieceType::PAWN), index)) return Piece::BLACK_PAWN;
        if(Bits::isSet(getPieces(Color::BLACK, PieceType::KNIGHT), index)) return Piece::BLACK_KNIGHT;
        if(Bits::isSet(getPieces(Color::BLACK, PieceType::BISHOP), index)) return Piece::BLACK_BISHOP;
        if(Bits::isSet(getPieces(Color::BLACK, PieceType::ROOK), index)) return Piece::BLACK_ROOK;
        if(Bits::isSet(getPieces(Color::BLACK, PieceType::QUEEN), index)) return Piece::BLACK_QUEEN;
        if(Bits::isSet(getPieces(Color::BLACK, PieceType::KING), index)) return Piece::BLACK_KING;

        return Piece::NONE;
    }

    [[nodiscard]] uint64_t getSide(Color color) const {
        return bb_sides[color.getValue()];
    }

    [[nodiscard]] uint64_t getOccupancy() const {
        return bb_sides[0] | bb_sides[1];
    }
    
    [[nodiscard]] uint8_t getKingIndex(Color color) const {
        uint64_t bb_king = getPieces(color, PieceType::KING);
        return Bits::popcount(bb_king);
    }

    void print() const {
        std::stringstream ss;
        uint8_t index = 56;

        ss << "---------------------------------\n";

        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                Piece piece = getPiece(index);
                ss << "| " << piece.getCharacter() << " ";

                index++;
            }
            ss << "|\n";
            ss << "---------------------------------\n";
            index -= 16;
        }

        std::cout << ss.str();
    }

    CastlingRights* getCastlingRights() {
        return &castling_rights;
    }

    Square* getEnPassantSquare() {
        return &en_passant_square;
    }

    void setEnPassantSquare(Square square) {
        this->en_passant_square = square;
    }

    void setSideToMove(Color color) {
        this->side_to_move = color;
    }

    uint8_t getSideToMove() {
        return this->side_to_move.getValue();
    }

    void setHalfMoveClock(const uint8_t &half_move) {
        this->half_move_clock = half_move;
    }

    [[nodiscard]] uint8_t getHalfMoveClock() const {
        return this->half_move_clock;
    }
};

#endif
