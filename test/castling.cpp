#include "gtest/gtest.h"
#include "../src/castling_rights.hpp"

class CastlingTest : public testing::Test {
protected:
    CastlingTest() = default;

    void SetUp() override {
        castling.set(Castling::WHITE_00);
        castling.set(Castling::WHITE_000);
        castling.set(Castling::BLACK_00);
        castling.set(Castling::BLACK_000);
    }

    Castling castling;
};

TEST(CastlingTestInit, Set) {
    Castling castling;

    ASSERT_EQ(castling.getCastlingRights(), 0);

    castling.set(Castling::WHITE_00);
    ASSERT_EQ(castling.getCastlingRights(), 1);

    castling.set(Castling::WHITE_000);
    ASSERT_EQ(castling.getCastlingRights(), 3);

    castling.set(Castling::BLACK_00);
    ASSERT_EQ(castling.getCastlingRights(), 7);

    castling.set(Castling::BLACK_000);
    ASSERT_EQ(castling.getCastlingRights(), 15);
}

TEST_F(CastlingTest, Has) {
    ASSERT_EQ(castling.has(Castling::WHITE_00), true);
    ASSERT_EQ(castling.has(Castling::WHITE_000), true);
    ASSERT_EQ(castling.has(Castling::BLACK_00), true);
    ASSERT_EQ(castling.has(Castling::BLACK_000), true);

    ASSERT_EQ(castling.has(Color::WHITE), true);
    ASSERT_EQ(castling.has(Color::BLACK), true);

    ASSERT_EQ(castling.hasNoCastling(), false);
}

TEST_F(CastlingTest, Unset) {
    castling.unset(Castling::WHITE_00);
    ASSERT_EQ(castling.getCastlingRights(), 14);

    castling.unset(Castling::WHITE_000);
    ASSERT_EQ(castling.getCastlingRights(), 12);

    castling.unset(Castling::BLACK_00);
    ASSERT_EQ(castling.getCastlingRights(), 8);

    castling.unset(Castling::BLACK_000);
    ASSERT_EQ(castling.getCastlingRights(), 0);
}

TEST_F(CastlingTest, UnsetColor) {
    castling.unset(Color::WHITE);
    ASSERT_EQ(castling.getCastlingRights(), 12);

    castling.unset(Color::BLACK);
    ASSERT_EQ(castling.getCastlingRights(), 0);
}