#include "gtest/gtest.h"

#include "../src/grid.hpp"

TEST(GridTest, FileGetBitboard) {
    ASSERT_EQ(File::getBitboard(0), File::FILE_ABB);
    ASSERT_EQ(File::getBitboard(1), File::FILE_BBB);
    ASSERT_EQ(File::getBitboard(2), File::FILE_CBB);
    ASSERT_EQ(File::getBitboard(3), File::FILE_DBB);
    ASSERT_EQ(File::getBitboard(4), File::FILE_EBB);
    ASSERT_EQ(File::getBitboard(5), File::FILE_FBB);
    ASSERT_EQ(File::getBitboard(6), File::FILE_GBB);
    ASSERT_EQ(File::getBitboard(7), File::FILE_HBB);
}

TEST(GridTest, RankGetBitboard) {
    ASSERT_EQ(Rank::getBitboard(0), Rank::RANK_1BB);
    ASSERT_EQ(Rank::getBitboard(1), Rank::RANK_2BB);
    ASSERT_EQ(Rank::getBitboard(2), Rank::RANK_3BB);
    ASSERT_EQ(Rank::getBitboard(3), Rank::RANK_4BB);
    ASSERT_EQ(Rank::getBitboard(4), Rank::RANK_5BB);
    ASSERT_EQ(Rank::getBitboard(5), Rank::RANK_6BB);
    ASSERT_EQ(Rank::getBitboard(6), Rank::RANK_7BB);
    ASSERT_EQ(Rank::getBitboard(7), Rank::RANK_8BB);
}

TEST(GridTest, GetPromoRank) {
    ASSERT_EQ(Rank::getPromoRank(Color::WHITE), Rank::RANK_8BB);
    ASSERT_EQ(Rank::getPromoRank(Color::BLACK), Rank::RANK_1BB);
    ASSERT_EQ(Rank::getPromoRank(Color::NONE), 0ULL);
}

TEST(GridTest, GetDoublePushRank) {
    ASSERT_EQ(Rank::getDoublePushRank(Color::WHITE), Rank::RANK_3BB);
    ASSERT_EQ(Rank::getDoublePushRank(Color::BLACK), Rank::RANK_6BB);
    ASSERT_EQ(Rank::getDoublePushRank(Color::NONE), 0ULL);
}

TEST(GridTest, SquareConstructor) {
    Square square_none("-");
    ASSERT_EQ(square_none.getValue(), Square::NONE);

    Square square_a1("a1");
    ASSERT_EQ(square_a1.getValue(), Square::Value::A1);

    Square square_h8("h8");
    ASSERT_EQ(square_h8.getValue(), Square::Value::H8);
}

TEST(GridTest, SquareOperatorLower) {
    Square square_a1("a1");
    Square square_none("-");

    ASSERT_EQ(square_a1 < 1, true);
    ASSERT_EQ(square_a1 < 0, false);

    ASSERT_EQ(square_none < 0, false);
    ASSERT_EQ(square_none < 64, false);
    ASSERT_EQ(square_none < 65, true);
}

TEST(GridTest, SquareOperatorPlusPlus) {
    Square square_a1("a1");
    Square square_h1("h1");
    Square square_h8("h8");

    square_a1++;
    ASSERT_EQ(square_a1.getValue(), Square::Value::B1);

    square_h1++;
    ASSERT_EQ(square_h1.getValue(), Square::Value::A2);

    square_h8++;
    ASSERT_EQ(square_h8.getValue(), Square::NONE);
}

TEST(GridTest, SquareIsValid) {
    Square square_a1("a1");
    Square square_h8("h8");
    Square square_none("-");

    ASSERT_EQ(square_a1.isValid(), true);
    ASSERT_EQ(square_h8.isValid(), true);
    ASSERT_EQ(square_none.isValid(), false);
}

TEST(GridTest, SquareToBitboard) {
    ASSERT_EQ(Square::toBitboard(0), 0x0000000000000001ULL);
    ASSERT_EQ(Square::toBitboard(1), 0x0000000000000002ULL);
    ASSERT_EQ(Square::toBitboard(2), 0x0000000000000004ULL);
    ASSERT_EQ(Square::toBitboard(63), 0x8000000000000000ULL);
}

TEST(GridTest, SquareGetEnPassantIndex) {
    ASSERT_EQ(Square::getEnPassantSquare(26), 18);
    ASSERT_EQ(Square::getEnPassantSquare(36), 44);
}

TEST(GridTest, SquareGetFileIndex) {
    Square square_a1("a1");
    Square square_b1("b1");
    Square square_c5("c5");
    Square square_h7("h7");

    ASSERT_EQ(square_a1.getFileIndex(), 0);
    ASSERT_EQ(square_b1.getFileIndex(), 1);
    ASSERT_EQ(square_c5.getFileIndex(), 2);
    ASSERT_EQ(square_h7.getFileIndex(), 7);
}

TEST(GridTest, SquareGetRankIndex) {
    Square square_a1("a1");
    Square square_b1("b1");
    Square square_d3("d3");
    Square square_e8("e8");

    ASSERT_EQ(square_a1.getRankIndex(), 0);
    ASSERT_EQ(square_b1.getRankIndex(), 0);
    ASSERT_EQ(square_d3.getRankIndex(), 2);
    ASSERT_EQ(square_e8.getRankIndex(), 7);
}