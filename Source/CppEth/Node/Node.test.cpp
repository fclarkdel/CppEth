#include <gtest/gtest.h>

#include <CppEth/Abi/Abi.hpp>
#include <CppEth/Node/Node.hpp>

TEST(Node, development)
{
	const size_t REQUEST_AMOUNT = 1000;

	CppEth::Node node("https://eth.llamarpc.com");

	std::vector<std::future<std::string>> futures;
	for(size_t count = 0; count < REQUEST_AMOUNT; ++count)
		futures.emplace_back(node.eth_call("0x11b815efB8f581194ae79006d24E0d814B7697F6", CppEth::Abi::encode("fee()")));

	node.send_requests();

	for(auto& future: futures)
		std::cout << future.get() << '\n';
}