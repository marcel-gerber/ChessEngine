#include "gtest/gtest.h"

#include "../src/piece.hpp"

TEST(PieceTest, OperatorMinus) {
    ASSERT_EQ(PieceType(PieceType::PAWN) - PieceType(PieceType::PAWN), static_cast<uint16_t>(PieceType::PAWN));
    ASSERT_EQ(PieceType(PieceType::KNIGHT) - PieceType(PieceType::PAWN), static_cast<uint16_t>(PieceType::KNIGHT));
    ASSERT_EQ(PieceType(PieceType::KNIGHT) - PieceType(PieceType::KNIGHT), static_cast<uint16_t>(PieceType::PAWN));
}

TEST(PieceTest, GetColor) {
    ASSERT_EQ(Piece(PieceType::PAWN, Color::WHITE).getColor(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::KNIGHT, Color::WHITE).getColor(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::BISHOP, Color::WHITE).getColor(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::ROOK, Color::WHITE).getColor(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::QUEEN, Color::WHITE).getColor(), Color::WHITE);
    ASSERT_EQ(Piece(PieceType::KING, Color::WHITE).getColor(), Color::WHITE);

    ASSERT_EQ(Piece(PieceType::PAWN, Color::BLACK).getColor(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::KNIGHT, Color::BLACK).getColor(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::BISHOP, Color::BLACK).getColor(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::ROOK, Color::BLACK).getColor(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::QUEEN, Color::BLACK).getColor(), Color::BLACK);
    ASSERT_EQ(Piece(PieceType::KING, Color::BLACK).getColor(), Color::BLACK);

    ASSERT_EQ(Piece(PieceType::NONE, Color::WHITE).getColor(), Color::NONE);
    ASSERT_EQ(Piece(PieceType::NONE, Color::BLACK).getColor(), Color::NONE);
}

TEST(PieceTest, GetType) {
    ASSERT_EQ(Piece(Piece::WHITE_PAWN).getType().getValue(), PieceType::PAWN);
    ASSERT_EQ(Piece(Piece::WHITE_KNIGHT).getType().getValue(), PieceType::KNIGHT);
    ASSERT_EQ(Piece(Piece::WHITE_BISHOP).getType().getValue(), PieceType::BISHOP);
    ASSERT_EQ(Piece(Piece::WHITE_ROOK).getType().getValue(), PieceType::ROOK);
    ASSERT_EQ(Piece(Piece::WHITE_QUEEN).getType().getValue(), PieceType::QUEEN);
    ASSERT_EQ(Piece(Piece::WHITE_KING).getType().getValue(), PieceType::KING);

    ASSERT_EQ(Piece(Piece::BLACK_PAWN).getType().getValue(), PieceType::PAWN);
    ASSERT_EQ(Piece(Piece::BLACK_KNIGHT).getType().getValue(), PieceType::KNIGHT);
    ASSERT_EQ(Piece(Piece::BLACK_BISHOP).getType().getValue(), PieceType::BISHOP);
    ASSERT_EQ(Piece(Piece::BLACK_ROOK).getType().getValue(), PieceType::ROOK);
    ASSERT_EQ(Piece(Piece::BLACK_QUEEN).getType().getValue(), PieceType::QUEEN);
    ASSERT_EQ(Piece(Piece::BLACK_KING).getType().getValue(), PieceType::KING);

    ASSERT_EQ(Piece(Piece::NONE).getType().getValue(), PieceType::NONE);
}

TEST(PieceTest, GetCharacter) {
    ASSERT_EQ(Piece('P').getCharacter(), 'P');
    ASSERT_EQ(Piece('N').getCharacter(), 'N');
    ASSERT_EQ(Piece('B').getCharacter(), 'B');
    ASSERT_EQ(Piece('R').getCharacter(), 'R');
    ASSERT_EQ(Piece('Q').getCharacter(), 'Q');
    ASSERT_EQ(Piece('K').getCharacter(), 'K');

    ASSERT_EQ(Piece('p').getCharacter(), 'p');
    ASSERT_EQ(Piece('n').getCharacter(), 'n');
    ASSERT_EQ(Piece('b').getCharacter(), 'b');
    ASSERT_EQ(Piece('r').getCharacter(), 'r');
    ASSERT_EQ(Piece('q').getCharacter(), 'q');
    ASSERT_EQ(Piece('k').getCharacter(), 'k');

    ASSERT_EQ(Piece('-').getCharacter(), ' ');
}