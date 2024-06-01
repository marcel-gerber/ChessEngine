#include "gtest/gtest.h"
#include "../src/castling_rights.hpp"

TEST(CastlingTest, Set) {
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