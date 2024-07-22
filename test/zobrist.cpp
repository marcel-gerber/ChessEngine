#include "gtest/gtest.h"

#include "../src/board.hpp"

TEST(ZobristTest, NormalMoves) {
    Board board = Board();
    board.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    const uint64_t initial_zobrist = board.getZobrist();

    Move move_w = Move::create<MoveType::NORMAL>(12, 28);
    board.makeMove(move_w);
    const uint64_t zobrist_after_move_w = board.getZobrist();

    Move move_b = Move::create<MoveType::NORMAL>(52, 36);
    board.makeMove(move_b);

    board.unmakeMove(move_b);
    ASSERT_EQ(board.getZobrist(), zobrist_after_move_w);

    board.unmakeMove(move_w);
    ASSERT_EQ(board.getZobrist(), initial_zobrist);
}

TEST(ZobristTest, PromotionMoves) {
    Board board = Board();
    board.setFen("8/6P1/2Q5/4p3/3qP3/5Q2/1q3PK1/qk6 w - - 0 73");
    const uint64_t initial_zobrist = board.getZobrist();

    Move move_w = Move::create<MoveType::PROMOTION>(54, 62, PieceType::QUEEN);
    board.makeMove(move_w);
    const uint64_t zobrist_after_move_w = board.getZobrist();

    Move move_b = Move::create<MoveType::NORMAL>(0, 16);
    board.makeMove(move_b);

    board.unmakeMove(move_b);
    ASSERT_EQ(board.getZobrist(), zobrist_after_move_w);

    board.unmakeMove(move_w);
    ASSERT_EQ(board.getZobrist(), initial_zobrist);
}

TEST(ZobristTest, EnPassantMoves) {
    Board board = Board();
    board.setFen("4k3/6p1/8/pP1pP3/7P/8/8/4K3 w - d6 0 6");
    const uint64_t initial_zobrist = board.getZobrist();

    Move move_w = Move::create<MoveType::EN_PASSANT>(36, 43);
    board.makeMove(move_w);
    const uint64_t zobrist_after_move_w = board.getZobrist();

    Move move_b = Move::create<MoveType::NORMAL>(54, 46);
    board.makeMove(move_b);

    board.unmakeMove(move_b);
    ASSERT_EQ(board.getZobrist(), zobrist_after_move_w);

    board.unmakeMove(move_w);
    ASSERT_EQ(board.getZobrist(), initial_zobrist);
}

TEST(ZobristTest, CastlingMoves) {
    Board board = Board();
    board.setFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    const uint64_t initial_zobrist = board.getZobrist();

    Move move_w = Move::create<MoveType::CASTLING>(4, 6);
    board.makeMove(move_w);
    const uint64_t zobrist_after_move_w = board.getZobrist();

    Move move_b = Move::create<MoveType::CASTLING>(60, 58);
    board.makeMove(move_b);

    board.unmakeMove(move_b);
    ASSERT_EQ(board.getZobrist(), zobrist_after_move_w);

    board.unmakeMove(move_w);
    ASSERT_EQ(board.getZobrist(), initial_zobrist);
}