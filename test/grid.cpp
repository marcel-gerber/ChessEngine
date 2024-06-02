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