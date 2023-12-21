#include <gtest/gtest.h>

#include <CppEth/Abi/Abi.hpp>
#include <CppEth/Types.hpp>

using namespace CppEth;

TEST(Abi, development)
{
	std::vector<address> expected_addresses{
		address("11b815efB8f581194ae79006d24E0d814B7697F6"),
		address("11b8ggdfB8f581194ae79006d24E0d814B7697F6"),
		address("11b815efB8f581194ae79006d24E0d814B7697F6"),
		address("11b815efB8fsd1194ae79006d24E0d814B7697F6"),
		address("11b815efB8f581194ae79006d24E0d814B7697F6"),
		address("11b815efB8f581194ae79006d24E0d814B7697F6"),
		address("11b815efB8f581194ae79006d24asd814B7697F6"),
		address("11b815efB8f581194ae79006d24E0d814B7697F6"),
		address("11b815efB8f581194asdfjhfdlskajhf32488777")};

	auto got_addresses = std::get<0>(Abi::decode<std::vector<address>>(Abi::encode("", expected_addresses)));

	for(int i = 0; i < expected_addresses.size(); ++i)
		EXPECT_EQ(expected_addresses[i].value(), got_addresses[i].value());
}

TEST(Abi_decode, should_be_prefix_insensitive)
{
	std::tuple expected = {uint256(42), int256(-42)};

	std::string unprefixed_encoding;
	unprefixed_encoding += "000000000000000000000000000000000000000000000000000000000000002a";
	unprefixed_encoding += "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd6";

	std::tuple returned = Abi::decode<uint256, int256>(unprefixed_encoding);

	EXPECT_EQ(expected, returned);

	std::string prefixedEncoding = "0x" + unprefixed_encoding;

	returned = Abi::decode<uint256, int256>(prefixedEncoding);

	EXPECT_EQ(expected, returned);
}

TEST(Abi_decode, shouldDecodeStaticTypes)
{
	std::tuple expected = {uint256(42), int256(-42)};

	std::string encoding = "0x";
	encoding += "000000000000000000000000000000000000000000000000000000000000002a";
	encoding += "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd6";

	std::tuple returned = Abi::decode<uint256, int256>(encoding);

	EXPECT_EQ(expected, returned);
}

TEST(Abi_decode, shouldDecodeDynamicTypes)
{
	std::tuple expected = {string("Lorem ipsum.")};

	std::string encoding = "0x";
	encoding += "0000000000000000000000000000000000000000000000000000000000000020";
	encoding += "000000000000000000000000000000000000000000000000000000000000000c";
	encoding += "4c6f72656d20697073756d2e0000000000000000000000000000000000000000";

	std::tuple returned = Abi::decode<string>(encoding);

	EXPECT_EQ(expected, returned);
}

TEST(Abi_decode, shouldDecodeMixedTypes)
{
	std::tuple expected = {uint256(42), int256(-42), string("Lorem ipsum.")};

	std::string encoding = "0x";
	encoding += "000000000000000000000000000000000000000000000000000000000000002a";
	encoding += "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd6";
	encoding += "0000000000000000000000000000000000000000000000000000000000000060";
	encoding += "000000000000000000000000000000000000000000000000000000000000000c";
	encoding += "4c6f72656d20697073756d2e0000000000000000000000000000000000000000";

	std::tuple returned = Abi::decode<uint256, int256, string>(encoding);

	EXPECT_EQ(expected, returned);
}

TEST(Abi_encode, shouldEncodeFunctionSignature)
{
	std::string expected = "0x";
	expected += "9d96e2df";

	std::string returned = Abi::encode("function()");

	EXPECT_EQ(expected, returned);
}

TEST(Abi_encode, shouldEncodeWithNoFunctionSignature)
{
	std::string expected = "0x";
	expected += "000000000000000000000000000000000000000000000000000000000000002a";

	std::string returned = Abi::encode<CppEth::uint256>("", 42);

	EXPECT_EQ(expected, returned);
}

TEST(Abi_encode, shouldEncodeStaticTypes)
{
	std::string expected = "0x";
	expected += "2237cdf4";
	expected += "000000000000000000000000000000000000000000000000000000000000002a";
	expected += "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd6";

	std::string returned = Abi::encode("function(uint256,int256)", uint256(42), int256(-42));

	EXPECT_EQ(expected, returned);
}

TEST(Abi_encode, shouldEncodeDynamicTypes)
{
	std::string expected = "0x";
	expected += "618ad2ae";
	expected += "0000000000000000000000000000000000000000000000000000000000000020";
	expected += "000000000000000000000000000000000000000000000000000000000000000c";
	expected += "4c6f72656d20697073756d2e0000000000000000000000000000000000000000";

	std::string returned = Abi::encode("function(string)", string("Lorem ipsum."));

	EXPECT_EQ(expected, returned);
}

TEST(Abi_encode, shouldEncodeMixedTypes)
{
	std::string expected = "0x";
	expected += "2f0e3bd0";
	expected += "000000000000000000000000000000000000000000000000000000000000002a";
	expected += "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd6";
	expected += "0000000000000000000000000000000000000000000000000000000000000060";
	expected += "000000000000000000000000000000000000000000000000000000000000000c";
	expected += "4c6f72656d20697073756d2e0000000000000000000000000000000000000000";

	std::string returned = Abi::encode("function(uint256,int256,string)", uint256(42), int256(-42),
									   string("Lorem ipsum."));

	EXPECT_EQ(expected, returned);

	expected = "0xa451f602";
	expected += "0000000000000000000000000000000000000000000000000000000000000060";
	expected += "00000000000000000000000000000000000000000000000000000000000000a0";
	expected += "000000000000000000000000000000000000000000000000000000003b9ac9ff";
	expected += "0000000000000000000000000000000000000000000000000000000000000003";
	expected += "4554480000000000000000000000000000000000000000000000000000000000";
	expected += "0000000000000000000000000000000000000000000000000000000000000004";
	expected += "5553445400000000000000000000000000000000000000000000000000000000";

	returned = Abi::encode("swap(string,string,uint256)", string("ETH"), string("USDT"),
						   uint256(999999999));

	EXPECT_EQ(expected, returned);
}