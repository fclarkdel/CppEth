#include <gtest/gtest.h>

#include <limits>
#include <type_traits>

#include <CppEth/Hex/Hex.hpp>
#include <CppEth/Types.hpp>

using namespace CppEth;

TEST(Hex_decode, shouldBeCaseInsensitive) {
    const string LOWERCASE_ENCODING = "4c6f72656d20697073756d2e";
    const string MIXEDCASE_ENCODING = "4C6f72656D20697073756d2E";
    const string UPPERCASE_ENCODING = "4C6F72656D20697073756D2E";

    const string ENCODING_VALUE = "Lorem ipsum.";

    EXPECT_EQ(ENCODING_VALUE, Hex::decode<string>(LOWERCASE_ENCODING));
    EXPECT_EQ(ENCODING_VALUE, Hex::decode<string>(MIXEDCASE_ENCODING));
    EXPECT_EQ(ENCODING_VALUE, Hex::decode<string>(UPPERCASE_ENCODING));
}

TEST(Hex_decode, shouldBePrefixInsensitive) {
    const string UNPREFIXED_ENCODING = "4C6F72656D20697073756D2E";
    const string LOWERCASE_PREFIXED_ENCODING = "0x4C6F72656D20697073756D2E";
    const string UPPERCASE_PREFIXED_ENCODING = "0X4C6F72656D20697073756D2E";

    const string ENCODING_VALUE = "Lorem ipsum.";

    EXPECT_EQ(ENCODING_VALUE, Hex::decode<string>(UNPREFIXED_ENCODING));
    EXPECT_EQ(ENCODING_VALUE, Hex::decode<string>(LOWERCASE_PREFIXED_ENCODING));
    EXPECT_EQ(ENCODING_VALUE, Hex::decode<string>(UPPERCASE_PREFIXED_ENCODING));
}

TEST(Hex_decode, shouldDecodeUint256) {
    const string ENCODING = "2a";
    const uint256 ENCODING_VALUE = 42;

    auto decodedValue = Hex::decode<uint256>(ENCODING);

    bool isUint256 = std::is_same<uint256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isUint256);

    EXPECT_EQ(ENCODING_VALUE, decodedValue);
}

TEST(Hex_decode, shouldDecodeMinAndMaxUint256) {
    const string MIN_VALUE_ENCODING = "0";
    const uint256 MIN_VALUE = std::numeric_limits<uint256>::min();

    auto decodedValue = Hex::decode<uint256>(MIN_VALUE_ENCODING);

    bool isUint256 = std::is_same<uint256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isUint256);

    EXPECT_EQ(MIN_VALUE, decodedValue);

    const string MAX_VALUE_ENCODING = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    const uint256 MAX_VALUE = std::numeric_limits<uint256>::max();

    decodedValue = Hex::decode<uint256>(MAX_VALUE_ENCODING);

    isUint256 = std::is_same<uint256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isUint256);

    EXPECT_EQ(MAX_VALUE, decodedValue);
}

TEST(Hex_decode, shouldDecodeInt256) {
    const string ENCODING = "d6";
    const int256 ENCODING_VALUE = -42;

    auto decodedValue = Hex::decode<int256>(ENCODING);

    bool isInt256 = std::is_same<int256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isInt256);

    EXPECT_EQ(ENCODING_VALUE, decodedValue);
}

TEST(Hex_decode, shouldDecodeMinAndMaxInt256) {
    const string MIN_VALUE_ENCODING = "8000000000000000000000000000000000000000000000000000000000000000";

    // boost::multiprecision provides an extra sign bit
    // for int types instead of using two's complement.
    // Thus we cannot use numeric_limits
    // to get the min of this value as it is in Solidity.
    // Instead we default to the minimum value representable by 256 bits
    // using two's complement, as is done in Solidity.
    const int256 MIN_VALUE = int256("-57896044618658097711785492504343953926634992332820282019728792003956564819968");

    auto decodedValue = Hex::decode<int256>(MIN_VALUE_ENCODING);

    bool isInt256 = std::is_same<int256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isInt256);

    EXPECT_EQ(MIN_VALUE, decodedValue);

    const string MAX_VALUE_ENCODING = "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    const int256 MAX_VALUE = int256("57896044618658097711785492504343953926634992332820282019728792003956564819967");

    decodedValue = Hex::decode<int256>(MAX_VALUE_ENCODING);

    isInt256 = std::is_same<int256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isInt256);

    EXPECT_EQ(MAX_VALUE, decodedValue);
}

TEST(Hex_decode, shouldDecodeAsciiString) {
    const string ENCODING = "4C6F72656D20697073756D2E";
    const string ENCODING_VALUE = "Lorem ipsum.";

    auto decodedValue = Hex::decode<string>(ENCODING);

    bool isInt256 = std::is_same<string, decltype(decodedValue)>::value;
    EXPECT_TRUE(isInt256);

    EXPECT_EQ(ENCODING_VALUE, decodedValue);
}

TEST(Hex_decode, shouldThrowOnInvalidHexDigits) {
    const string INVALID_HEX_DIGIT_ENCODING = "googletestiscool";

    EXPECT_THROW(Hex::decode<uint256>(INVALID_HEX_DIGIT_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<int256>(INVALID_HEX_DIGIT_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<string>(INVALID_HEX_DIGIT_ENCODING), std::invalid_argument);
}

TEST(Hex_decode, shouldThrowOnEmptyHexData) {
    const string EMPTY_UNPREFIXED_ENCODING;

    EXPECT_THROW(Hex::decode<uint256>(EMPTY_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<int256>(EMPTY_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<string>(EMPTY_UNPREFIXED_ENCODING), std::invalid_argument);

    const string EMPTY_LOWERCASE_UNPREFIXED_ENCODING = "0x";

    EXPECT_THROW(Hex::decode<uint256>(EMPTY_LOWERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<int256>(EMPTY_LOWERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<string>(EMPTY_LOWERCASE_UNPREFIXED_ENCODING), std::invalid_argument);

    const string EMPTY_UPPERCASE_UNPREFIXED_ENCODING = "0X";

    EXPECT_THROW(Hex::decode<uint256>(EMPTY_UPPERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<int256>(EMPTY_UPPERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<string>(EMPTY_UPPERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
}

TEST(Hex_encode, shouldEncodeUint256) {
    const string ENCODING = "2a";
    const uint256 ENCODING_VALUE = 42;

    auto encodedValue = Hex::encode(ENCODING_VALUE);

    EXPECT_EQ(ENCODING, encodedValue);
}

TEST(Hex_encode, shouldEncodeInt256) {
    const string ENCODING = "d6";
    const int256 ENCODING_VALUE = -42;

    auto encodedValue = Hex::encode(ENCODING_VALUE);

    EXPECT_EQ(ENCODING, encodedValue);
}

TEST(Hex_encode, shouldEncodeAsciiString) {
    const string ENCODING = "4c6f72656d20697073756d2e";
    const string ENCODING_VALUE = "Lorem ipsum.";

    auto encodedValue = Hex::encode(ENCODING_VALUE);

    EXPECT_EQ(ENCODING, encodedValue);
}

TEST(Hex, shouldBeInversesOfEachOther){
    const string ENCODING = "4c6f72656d20697073756d2e";
    const string ENCODING_VALUE = "Lorem ipsum.";

    EXPECT_EQ(Hex::encode(Hex::decode<string>(ENCODING)), ENCODING);
    EXPECT_EQ(Hex::decode<string>(Hex::encode(ENCODING_VALUE)), ENCODING_VALUE);
}