#include "gtest/gtest.h"

#include "../src/utils/bits.hpp"

TEST(BitsTest, Set) {
    uint64_t bits = 0ULL;

    Bits::set(bits, 0);
    ASSERT_EQ(bits, 0x0000000000000001ULL);

    Bits::set(bits, 1);
    ASSERT_EQ(bits, 0x0000000000000003ULL);

    Bits::set(bits, 2);
    ASSERT_EQ(bits, 0x0000000000000007ULL);

    Bits::set(bits, 3);
    ASSERT_EQ(bits, 0x000000000000000FULL);

    Bits::set(bits, 63);
    ASSERT_EQ(bits, 0x800000000000000FULL);
}

TEST(BitsTest, Unset) {
    uint64_t bits = 0x800000000000000FULL;

    Bits::unset(bits, 63);
    ASSERT_EQ(bits, 0x000000000000000FULL);

    Bits::unset(bits, 3);
    ASSERT_EQ(bits, 0x0000000000000007ULL);

    Bits::unset(bits, 2);
    ASSERT_EQ(bits, 0x0000000000000003ULL);

    Bits::unset(bits, 1);
    ASSERT_EQ(bits, 0x0000000000000001ULL);

    Bits::unset(bits, 0);
    ASSERT_EQ(bits, 0ULL);
}

TEST(BitsTest, IsSet) {
    uint64_t bits = 0x800000000000000FULL;

    ASSERT_EQ(Bits::isSet(bits, 0), true);
    ASSERT_EQ(Bits::isSet(bits, 1), true);
    ASSERT_EQ(Bits::isSet(bits, 2), true);
    ASSERT_EQ(Bits::isSet(bits, 3), true);
    ASSERT_EQ(Bits::isSet(bits, 4), false);
    ASSERT_EQ(Bits::isSet(bits, 42), false);
    ASSERT_EQ(Bits::isSet(bits, 63), true);
}

TEST(BitsTest, Popcount) {
    uint64_t bits = 0x0000000000000001ULL;
    ASSERT_EQ(Bits::popcount(bits), 1);

    bits = 0x0000000000000003ULL;
    ASSERT_EQ(Bits::popcount(bits), 2);

    bits = 0x0000000000000007ULL;
    ASSERT_EQ(Bits::popcount(bits), 3);

    bits = 0x800000000000000FULL;
    ASSERT_EQ(Bits::popcount(bits), 5);
}

TEST(BitsTest, Lsb) {
    uint64_t bits = 0x0000000000000001ULL;
    ASSERT_EQ(Bits::lsb(bits), 0);

    bits = 0x0000000000000002ULL;
    ASSERT_EQ(Bits::lsb(bits), 1);

    bits = 0x0000000000000004ULL;
    ASSERT_EQ(Bits::lsb(bits), 2);

    bits = 0x0000000001000000ULL;
    ASSERT_EQ(Bits::lsb(bits), 24);

    bits = 0x8000000000000000ULL;
    ASSERT_EQ(Bits::lsb(bits), 63);
}

TEST(BitsTest, Pop) {
    uint64_t bits = 0x0000000000000001ULL;
    Bits::pop(bits);
    ASSERT_EQ(bits, 0ULL);

    bits = 0x0000000000000003ULL;
    Bits::pop(bits);
    ASSERT_EQ(bits, 0x0000000000000002ULL);

    bits = 0x000000000000000AULL;
    Bits::pop(bits);
    ASSERT_EQ(bits, 0x0000000000000008ULL);
}

TEST(BitsTest, GetIndices) {
    uint64_t bits = 0b101010ULL;
    std::vector<uint8_t> expected_indices = {1, 3, 5};
    ASSERT_EQ(Bits::getIndices(bits), expected_indices);

    bits = 0x8000000000000000ULL;
    expected_indices = {63};
    ASSERT_EQ(Bits::getIndices(bits), expected_indices);

    bits = 0b1000000010001ULL;
    expected_indices = {0, 4, 12};
    ASSERT_EQ(Bits::getIndices(bits), expected_indices);
}