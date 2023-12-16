#ifndef CPPETH_NODE_HPP
#define CPPETH_NODE_HPP

#include <future>
#include <string>

#include <LibJsonRpc.hpp>
namespace CppEth
{
class Node
{
public:
	explicit Node(const std::string& url);

	std::string
	get_url();

	void
	set_url(const std::string& url);

	std::future<std::string>
	eth_call(const std::string& to, const std::string& data);

	std::future<std::string>
	eth_blockNumber();

	void
	send_requests();

private:
	std::string url;

	LibJsonRpc::Server json_rpc_server;
};
}
#endif