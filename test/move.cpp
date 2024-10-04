#include "gtest/gtest.h"

#include "../src/move.hpp"

TEST(MoveTest, FromIndex) {
    ASSERT_EQ(Move::create<MoveType::NORMAL>(0, 8).fromIndex(), 0);
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(21, 42).fromIndex(), 21);
    ASSERT_EQ(Move::create<MoveType::CASTLING>(7, 23).fromIndex(), 7);
    ASSERT_EQ(Move::create<MoveType::EN_PASSANT>(44, 55).fromIndex(), 44);
}

TEST(MoveTest, ToIndex) {
    ASSERT_EQ(Move::create<MoveType::NORMAL>(0, 8).toIndex(), 8);
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(21, 42).toIndex(), 42);
    ASSERT_EQ(Move::create<MoveType::CASTLING>(7, 23).toIndex(), 23);
    ASSERT_EQ(Move::create<MoveType::EN_PASSANT>(44, 55).toIndex(), 55);
}

TEST(MoveTest, MoveType) {
    ASSERT_EQ(Move::create<MoveType::NORMAL>(0, 8).type(), MoveType::NORMAL);
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(21, 42).type(), MoveType::PROMOTION);
    ASSERT_EQ(Move::create<MoveType::CASTLING>(7, 23).type(), MoveType::CASTLING);
    ASSERT_EQ(Move::create<MoveType::EN_PASSANT>(44, 55).type(), MoveType::EN_PASSANT);
}

TEST(MoveTest, PromotionType) {
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(48, 56).promotionType(), static_cast<uint8_t>(PieceType::KNIGHT));
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(48, 56, PieceType::BISHOP).promotionType(), static_cast<uint8_t>(PieceType::BISHOP));
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(48, 56, PieceType::ROOK).promotionType(), static_cast<uint8_t>(PieceType::ROOK));
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(48, 56, PieceType::QUEEN).promotionType(), static_cast<uint8_t>(PieceType::QUEEN));
}