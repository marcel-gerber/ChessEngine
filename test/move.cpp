#include "gtest/gtest.h"
#include "../src/move.hpp"

TEST(MoveTest, FromIndex) {
    ASSERT_EQ(Move::create<MoveType::NORMAL>(0, 8).from_index(), 0);
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(21, 42).from_index(), 21);
    ASSERT_EQ(Move::create<MoveType::CASTLING>(7, 23).from_index(), 7);
    ASSERT_EQ(Move::create<MoveType::EN_PASSANT>(44, 55).from_index(), 44);
}

TEST(MoveTest, ToIndex) {
    ASSERT_EQ(Move::create<MoveType::NORMAL>(0, 8).to_index(), 8);
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(21, 42).to_index(), 42);
    ASSERT_EQ(Move::create<MoveType::CASTLING>(7, 23).to_index(), 23);
    ASSERT_EQ(Move::create<MoveType::EN_PASSANT>(44, 55).to_index(), 55);
}

TEST(MoveTest, MoveType) {
    ASSERT_EQ(Move::create<MoveType::NORMAL>(0, 8).type(), MoveType::NORMAL);
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(21, 42).type(), MoveType::PROMOTION);
    ASSERT_EQ(Move::create<MoveType::CASTLING>(7, 23).type(), MoveType::CASTLING);
    ASSERT_EQ(Move::create<MoveType::EN_PASSANT>(44, 55).type(), MoveType::EN_PASSANT);
}

TEST(MoveTest, PromotionType) {
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(48, 56).promotion_type(), static_cast<uint8_t>(PieceType::KNIGHT));
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(48, 56, PieceType::BISHOP).promotion_type(), static_cast<uint8_t>(PieceType::BISHOP));
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(48, 56, PieceType::ROOK).promotion_type(), static_cast<uint8_t>(PieceType::ROOK));
    ASSERT_EQ(Move::create<MoveType::PROMOTION>(48, 56, PieceType::QUEEN).promotion_type(), static_cast<uint8_t>(PieceType::QUEEN));
}