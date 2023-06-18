#include <gtest/gtest.h>

#include <CppEth/Format/Format.hpp>

using namespace CppEth;

TEST(Format_trimPrefixIfPresent, shouldTrimPrefix) {
    const std::string UNPREFIXED = "4c6f72656d20697073756d2e";
    const std::string PREFIXED = "0x4c6f72656d20697073756d2e";

    EXPECT_EQ(UNPREFIXED, Format::trimPrefixIfPresent(PREFIXED));
    EXPECT_EQ(UNPREFIXED, Format::trimPrefixIfPresent(UNPREFIXED));
}

TEST(Format_padLeft, shouldPadLeft){
    const char PADDING_CHAR = '0';
    const size_t PADDING_AMOUNT = 10;

    std::string unpadded = "unpadded";

    std::string padding(PADDING_AMOUNT, PADDING_CHAR);

    std::string padded = padding + unpadded;

    EXPECT_EQ(padded, Format::padLeft(PADDING_CHAR, PADDING_AMOUNT, unpadded));
}

TEST(Format_padRight, shouldPadRight){
    const char PADDING_CHAR = '0';
    const size_t PADDING_AMOUNT = 10;

    std::string unpadded = "unpadded";

    std::string padding(PADDING_AMOUNT, PADDING_CHAR);

    std::string padded = unpadded + padding;

    EXPECT_EQ(padded, Format::padRight(PADDING_CHAR, PADDING_AMOUNT, unpadded));
}