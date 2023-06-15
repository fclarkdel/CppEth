#include <limits>
#include <type_traits>

#include "gtest/gtest.h"

#include "Types.hpp"
#include "Hex.hpp"

using namespace SolidityContractInterface;

TEST(Hex_decode, shouldBeCaseInsensitive) {
    const Types::string LOWERCASE_ENCODING = "4c6f72656d20697073756d2e";
    const Types::string MIXEDCASE_ENCODING = "4C6f72656D20697073756d2E";
    const Types::string UPPERCASE_ENCODING = "4C6F72656D20697073756D2E";

    const Types::string ENCODING_VALUE = "Lorem ipsum.";

    EXPECT_EQ(ENCODING_VALUE, Hex::decode<Types::string>(LOWERCASE_ENCODING));
    EXPECT_EQ(ENCODING_VALUE, Hex::decode<Types::string>(MIXEDCASE_ENCODING));
    EXPECT_EQ(ENCODING_VALUE, Hex::decode<Types::string>(UPPERCASE_ENCODING));
}

TEST(Hex_decode, shouldBePrefixInsensitive) {
    const Types::string UNPREFIXED_ENCODING = "4C6F72656D20697073756D2E";
    const Types::string LOWERCASE_PREFIXED_ENCODING = "0x4C6F72656D20697073756D2E";
    const Types::string UPPERCASE_PREFIXED_ENCODING = "0X4C6F72656D20697073756D2E";

    const Types::string ENCODING_VALUE = "Lorem ipsum.";

    EXPECT_EQ(ENCODING_VALUE, Hex::decode<Types::string>(UNPREFIXED_ENCODING));
    EXPECT_EQ(ENCODING_VALUE, Hex::decode<Types::string>(LOWERCASE_PREFIXED_ENCODING));
    EXPECT_EQ(ENCODING_VALUE, Hex::decode<Types::string>(UPPERCASE_PREFIXED_ENCODING));
}

TEST(Hex_decode, shouldDecodeUint256) {
    const Types::string ENCODING = "2a";
    const Types::uint256 ENCODING_VALUE = 42;

    auto decodedValue = Hex::decode<Types::uint256>(ENCODING);

    bool isUint256 = std::is_same<Types::uint256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isUint256);

    EXPECT_EQ(ENCODING_VALUE, decodedValue);
}

TEST(Hex_decode, shouldDecodeMinAndMaxUint256) {
    const Types::string MIN_VALUE_ENCODING = "0";
    const Types::uint256 MIN_VALUE = std::numeric_limits<Types::uint256>::min();

    auto decodedValue = Hex::decode<Types::uint256>(MIN_VALUE_ENCODING);

    bool isUint256 = std::is_same<Types::uint256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isUint256);

    EXPECT_EQ(MIN_VALUE, decodedValue);

    const Types::string MAX_VALUE_ENCODING = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    const Types::uint256 MAX_VALUE = std::numeric_limits<Types::uint256>::max();

    decodedValue = Hex::decode<Types::uint256>(MAX_VALUE_ENCODING);

    isUint256 = std::is_same<Types::uint256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isUint256);

    EXPECT_EQ(MAX_VALUE, decodedValue);
}

TEST(Hex_decode, shouldDecodeInt256) {
    const Types::string ENCODING = "d6";
    const Types::int256 ENCODING_VALUE = -42;

    auto decodedValue = Hex::decode<Types::int256>(ENCODING);

    bool isInt256 = std::is_same<Types::int256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isInt256);

    EXPECT_EQ(ENCODING_VALUE, decodedValue);
}

TEST(Hex_decode, shouldDecodeMinAndMaxInt256) {
    const Types::string MIN_VALUE_ENCODING = "8000000000000000000000000000000000000000000000000000000000000000";

    // boost::multiprecision provides an extra sign bit
    // for int types instead of using two's complement.
    // Thus we cannot use numeric_limits
    // to get the min of this value as it is in Solidity.
    // Instead we default to the minimum value representable by 256 bits
    // using two's complement, as is done in Solidity.
    const Types::int256 MIN_VALUE = Types::int256("-57896044618658097711785492504343953926634992332820282019728792003956564819968");

    auto decodedValue = Hex::decode<Types::int256>(MIN_VALUE_ENCODING);

    bool isInt256 = std::is_same<Types::int256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isInt256);

    EXPECT_EQ(MIN_VALUE, decodedValue);

    const Types::string MAX_VALUE_ENCODING = "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    const Types::int256 MAX_VALUE = Types::int256("57896044618658097711785492504343953926634992332820282019728792003956564819967");

    decodedValue = Hex::decode<Types::int256>(MAX_VALUE_ENCODING);

    isInt256 = std::is_same<Types::int256, decltype(decodedValue)>::value;
    EXPECT_TRUE(isInt256);

    EXPECT_EQ(MAX_VALUE, decodedValue);
}

TEST(Hex_decode, shouldDecodeAsciiString) {
    const Types::string ENCODING = "4C6F72656D20697073756D2E";
    const Types::string ENCODING_VALUE = "Lorem ipsum.";

    auto decodedValue = Hex::decode<Types::string>(ENCODING);

    bool isInt256 = std::is_same<Types::string, decltype(decodedValue)>::value;
    EXPECT_TRUE(isInt256);

    EXPECT_EQ(ENCODING_VALUE, decodedValue);
}

TEST(Hex_decode, shouldThrowOnInvalidHexDigits) {
    const Types::string INVALID_HEX_DIGIT_ENCODING = "googletestiscool";

    EXPECT_THROW(Hex::decode<Types::uint256>(INVALID_HEX_DIGIT_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<Types::int256>(INVALID_HEX_DIGIT_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<Types::string>(INVALID_HEX_DIGIT_ENCODING), std::invalid_argument);
}

TEST(Hex_decode, shouldThrowOnEmptyHexData) {
    const Types::string EMPTY_UNPREFIXED_ENCODING;

    EXPECT_THROW(Hex::decode<Types::uint256>(EMPTY_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<Types::int256>(EMPTY_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<Types::string>(EMPTY_UNPREFIXED_ENCODING), std::invalid_argument);

    const Types::string EMPTY_LOWERCASE_UNPREFIXED_ENCODING = "0x";

    EXPECT_THROW(Hex::decode<Types::uint256>(EMPTY_LOWERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<Types::int256>(EMPTY_LOWERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<Types::string>(EMPTY_LOWERCASE_UNPREFIXED_ENCODING), std::invalid_argument);

    const Types::string EMPTY_UPPERCASE_UNPREFIXED_ENCODING = "0X";

    EXPECT_THROW(Hex::decode<Types::uint256>(EMPTY_UPPERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<Types::int256>(EMPTY_UPPERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
    EXPECT_THROW(Hex::decode<Types::string>(EMPTY_UPPERCASE_UNPREFIXED_ENCODING), std::invalid_argument);
}

TEST(Hex_encode, shouldEncodeUint256) {
    const Types::string ENCODING = "2a";
    const Types::uint256 ENCODING_VALUE = 42;

    auto encodedValue = Hex::encode(ENCODING_VALUE);

    EXPECT_EQ(ENCODING, encodedValue);
}

TEST(Hex_encode, shouldEncodeInt256) {
    const Types::string ENCODING = "d6";
    const Types::int256 ENCODING_VALUE = -42;

    auto encodedValue = Hex::encode(ENCODING_VALUE);

    EXPECT_EQ(ENCODING, encodedValue);
}

TEST(Hex_encode, shouldEncodeAsciiString) {
    const Types::string ENCODING = "4c6f72656d20697073756d2e";
    const Types::string ENCODING_VALUE = "Lorem ipsum.";

    auto encodedValue = Hex::encode(ENCODING_VALUE);

    EXPECT_EQ(ENCODING, encodedValue);
}

TEST(Hex, shouldBeInversesOfEachOther){
    const Types::string ENCODING = "4c6f72656d20697073756d2e";
    const Types::string ENCODING_VALUE = "Lorem ipsum.";

    EXPECT_EQ(Hex::encode(Hex::decode<Types::string>(ENCODING)), ENCODING);
    EXPECT_EQ(Hex::decode<Types::string>(Hex::encode(ENCODING_VALUE)), ENCODING_VALUE);
}