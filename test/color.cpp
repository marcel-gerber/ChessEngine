#include "gtest/gtest.h"
#include "../src/color.hpp"

class ColorTest : public testing::Test {
protected:
    ColorTest() = default;

    void SetUp() override {
        color_w = Color(Color::WHITE);
        color_b = Color(Color::BLACK);
        color_none = Color(Color::NONE);
    }

    Color color_w{};
    Color color_b{};
    Color color_none{};
};

TEST_F(ColorTest, GetOppositeColor) {
    ASSERT_EQ(color_w.getOppositeColor(), Color::BLACK);
    ASSERT_EQ(color_b.getOppositeColor(), Color::WHITE);
}

TEST_F(ColorTest, GetValue) {
    ASSERT_EQ(color_w.getValue(), 0);
    ASSERT_EQ(color_b.getValue(), 1);
    ASSERT_EQ(color_none.getValue(), 2);
}

TEST_F(ColorTest, EqualityOperator) {
    ASSERT_EQ(color_w == color_w, true);
    ASSERT_EQ(color_b == color_b, true);
    ASSERT_EQ(color_none == color_none, true);

    ASSERT_EQ(color_b == color_w, false);
    ASSERT_EQ(color_w == color_b, false);

    ASSERT_EQ(color_none == color_w, false);
    ASSERT_EQ(color_none == color_b, false);
    ASSERT_EQ(color_w == color_none, false);
    ASSERT_EQ(color_b == color_none, false);
}