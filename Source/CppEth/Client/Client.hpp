#ifndef CPPETH_CLIENT_HPP
#define CPPETH_CLIENT_HPP

#include <future>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>

#include <curl/curl.h>

namespace CppEth
{
class Client
{
public:
    Client(std::string url);

    ~Client();

    std::future<std::string> async_call(const std::string& to, const std::string& data);

private:
    inline static std::mutex shared_lock;

    inline static size_t live_clients;

    const std::string URL;

    CURL* base_request;

    curl_slist* base_header_list;

    CURLM* multi_request;

    CURLSH* shared_cache;

    bool client_is_live;

    std::thread client_thread;

    std::unordered_map<CURL*, std::promise<std::string>> map_request_to_promise;

    void request_consumer();
};
}
#endif