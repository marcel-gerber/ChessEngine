#include "gtest/gtest.h"
#include "../src/board.hpp"
#include "../src/fen_parser.hpp"

TEST(BoardTest, MakeAndUnmakeMove) {
    Board board = Board();
    Fen::setFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Move move_w = Move::create<MoveType::NORMAL>(12, 28);
    board.makeMove(move_w);
    ASSERT_EQ(board.getPiece(12).getValue(), static_cast<uint8_t>(Piece::NONE));
    ASSERT_EQ(board.getPiece(28).getValue(), static_cast<uint8_t>(Piece::WHITE_PAWN));

    Move move_b = Move::create<MoveType::NORMAL>(52, 36);
    board.makeMove(move_b);
    ASSERT_EQ(board.getPiece(52).getValue(), static_cast<uint8_t>(Piece::NONE));
    ASSERT_EQ(board.getPiece(36).getValue(), static_cast<uint8_t>(Piece::BLACK_PAWN));

    board.unmakeMove(move_b);
    ASSERT_EQ(board.getPiece(52).getValue(), static_cast<uint8_t>(Piece::BLACK_PAWN));
    ASSERT_EQ(board.getPiece(36).getValue(), static_cast<uint8_t>(Piece::NONE));

    board.unmakeMove(move_w);
    ASSERT_EQ(board.getPiece(12).getValue(), static_cast<uint8_t>(Piece::WHITE_PAWN));
    ASSERT_EQ(board.getPiece(28).getValue(), static_cast<uint8_t>(Piece::NONE));
}