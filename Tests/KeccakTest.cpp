#include "gtest/gtest.h"

#include "Keccak.hpp"

using namespace SolidityContractInterface;

TEST(Keccak_keccak256, shouldReturnKeccak256Hash) {
    std::string expected = "D3BD66DA6C645D999A9CB9BD7D5EFBC495988B9749534F0988847E8EC7A11957";
    std::string returned = Keccak::keccak256("Lorem ipsum.");

    EXPECT_EQ(expected, returned);
}