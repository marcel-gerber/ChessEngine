#include "gtest/gtest.h"

#include "../src/piece.hpp"

TEST(PieceTest, OperatorMinus) {
    ASSERT_EQ(PieceType(PieceType::PAWN) - PieceType(PieceType::PAWN), static_cast<uint16_t>(PieceType::PAWN));
    ASSERT_EQ(PieceType(PieceType::KNIGHT) - PieceType(PieceType::PAWN), static_cast<uint16_t>(PieceType::KNIGHT));
    ASSERT_EQ(PieceType(PieceType::KNIGHT) - PieceType(PieceType::KNIGHT), static_cast<uint16_t>(PieceType::PAWN));
}

TEST(PieceTest, GetColor) {
    ASSERT_EQ(Piece(PieceType::PAWN, Color::WHITE).color(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::KNIGHT, Color::WHITE).color(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::BISHOP, Color::WHITE).color(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::ROOK, Color::WHITE).color(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::QUEEN, Color::WHITE).color(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::KING, Color::WHITE).color(), Color::WHITE);

    ASSERT_EQ(Piece(PieceType::PAWN, Color::BLACK).color(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::KNIGHT, Color::BLACK).color(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::BISHOP, Color::BLACK).color(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::ROOK, Color::BLACK).color(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::QUEEN, Color::BLACK).color(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::KING, Color::BLACK).color(), Color::BLACK);

    ASSERT_EQ(Piece(PieceType::NONE, Color::WHITE).color(), Color::NONE);
    ASSERT_EQ(Piece(PieceType::NONE, Color::BLACK).color(), Color::NONE);
}

TEST(PieceTest, GetType) {
    ASSERT_EQ(Piece(Piece::WHITE_PAWN).type().value(), PieceType::PAWN);
    ASSERT_EQ(Piece(Piece::WHITE_KNIGHT).type().value(), PieceType::KNIGHT);
    ASSERT_EQ(Piece(Piece::WHITE_BISHOP).type().value(), PieceType::BISHOP);
    ASSERT_EQ(Piece(Piece::WHITE_ROOK).type().value(), PieceType::ROOK);
    ASSERT_EQ(Piece(Piece::WHITE_QUEEN).type().value(), PieceType::QUEEN);
    ASSERT_EQ(Piece(Piece::WHITE_KING).type().value(), PieceType::KING);

    ASSERT_EQ(Piece(Piece::BLACK_PAWN).type().value(), PieceType::PAWN);
    ASSERT_EQ(Piece(Piece::BLACK_KNIGHT).type().value(), PieceType::KNIGHT);
    ASSERT_EQ(Piece(Piece::BLACK_BISHOP).type().value(), PieceType::BISHOP);
    ASSERT_EQ(Piece(Piece::BLACK_ROOK).type().value(), PieceType::ROOK);
    ASSERT_EQ(Piece(Piece::BLACK_QUEEN).type().value(), PieceType::QUEEN);
    ASSERT_EQ(Piece(Piece::BLACK_KING).type().value(), PieceType::KING);

    ASSERT_EQ(Piece(Piece::NONE).type().value(), PieceType::NONE);
}

TEST(PieceTest, GetCharacter) {
    ASSERT_EQ(Piece('P').character(), 'P');
    ASSERT_EQ(Piece('N').character(), 'N');
    ASSERT_EQ(Piece('B').character(), 'B');
    ASSERT_EQ(Piece('R').character(), 'R');
    ASSERT_EQ(Piece('Q').character(), 'Q');
    ASSERT_EQ(Piece('K').character(), 'K');

    ASSERT_EQ(Piece('p').character(), 'p');
    ASSERT_EQ(Piece('n').character(), 'n');
    ASSERT_EQ(Piece('b').character(), 'b');
    ASSERT_EQ(Piece('r').character(), 'r');
    ASSERT_EQ(Piece('q').character(), 'q');
    ASSERT_EQ(Piece('k').character(), 'k');

    ASSERT_EQ(Piece('-').character(), ' ');
}