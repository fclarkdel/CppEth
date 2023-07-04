#include <CppEth/Node/Node.hpp>

namespace CppEth
{
Node::Node(const std::string& url):
	url{url},
	json_rpc_server{url}
{
}
std::string
Node::get_url()
{
	return url;
}
void
Node::set_url(const std::string& url)
{
	this->url = url;
	json_rpc_server.set_url(url);
}
std::future<std::string>
Node::eth_call(const std::string& to, const std::string& data)
{
	std::string params = R"([{"to":")";
	params += (to);
	params += R"(", "data":")";
	params += data;
	params += R"("}, "safe"])";

	return json_rpc_server.batch_request("eth_call", params);
}
void
Node::send_requests()
{
	json_rpc_server.execute_batch();
}
}