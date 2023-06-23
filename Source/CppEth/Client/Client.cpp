#include <CppEth/Client/Client.hpp>

#include <functional>
#include <utility>
#include <iostream>

void printRequestInfo(CURL* request){
    double total_time = 0;
    curl_easy_getinfo(request, CURLINFO_TOTAL_TIME, &total_time);

    double namelookup_time = 0;
    curl_easy_getinfo(request, CURLINFO_NAMELOOKUP_TIME, &namelookup_time);

    double connect_time = 0;
    curl_easy_getinfo(request, CURLINFO_CONNECT_TIME, &connect_time);

    double appconnect_time = 0;
    curl_easy_getinfo(request, CURLINFO_APPCONNECT_TIME, &appconnect_time);

    double pretransfer_time = 0;
    curl_easy_getinfo(request, CURLINFO_PRETRANSFER_TIME, &pretransfer_time);

    double starttransfer_time = 0;
    curl_easy_getinfo(request, CURLINFO_STARTTRANSFER_TIME, &starttransfer_time);

    double redirect_time = 0;
    curl_easy_getinfo(request, CURLINFO_REDIRECT_TIME, &redirect_time);

    std::cout << "total_time: " << total_time << "s\n"
              << "\tnamelookup_time: " << namelookup_time << "s\n"
              << "\tconnect_time: " << connect_time << "s\n"
              << "\tappconnect_time: " << appconnect_time << "s\n"
              << "\tpretransfer_time: " << pretransfer_time << "s\n"
              << "\tstarttransfer_time: " << starttransfer_time << "s\n"
              << "\tredirect_time: " << redirect_time << "s\n";
}

CppEth::Client::Client(std::string url) :
        URL{std::move(url)},
        base_request{curl_easy_init()},
        base_header_list{curl_slist_append(nullptr, "Content-Type: application/json")},
        multi_request{curl_multi_init()},
        shared_cache{curl_share_init()}
{
    // Critical section.
    // Ensures that curl_global_init()
    // is called once by
    // all current client instances.
    {
        std::lock_guard<std::mutex> lock_guard(shared_lock);

        // True when this will be
        // the only 'Client' alive at the moment.
        //
        // Note:
        //  'live_clients' need not be 'std::atomic<size_t>'
        //  since it is only modified in the critical sections.
        if (live_clients == 0)
            curl_global_init(CURL_GLOBAL_ALL);

        client_is_live = true;
        ++live_clients;
    }
    curl_easy_setopt(
            base_request,
            CURLOPT_URL,
            URL.c_str()
    );
    curl_easy_setopt(
            base_request,
            CURLOPT_HTTPHEADER,
            base_header_list
    );
    curl_easy_setopt(
            base_request,
            CURLOPT_WRITEFUNCTION,
            // '+' before lambda converts it from
            // a non-captured lambda object to a
            // a plain C pointer.
            //
            // Note:
            //  This only works for non-capturing lambdas.
            +[](char *ptr, size_t size, size_t nmemb, void* userdata) {
                size_t total_size = size * nmemb;

                auto promise = static_cast<std::promise<std::string>*>(userdata);

                promise->set_value(std::string(ptr, total_size));

                return total_size;
            }
    );
    curl_easy_setopt(base_request, CURLOPT_TIMEOUT_MS, 2000);

    curl_share_setopt(shared_cache, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);

    // Must start 'client_thread'
    // after 'libcurl' stuff has been initialized
    // and 'client_is_live' has been set to true
    // - that's why it's down here.
    client_thread = std::thread(
        // Lambda provides 'Callable' access to
        // member function 'request_consumer'.
        [this]()
        {
            return this->request_consumer();
        }
    );
}

CppEth::Client::~Client() {
    // Critical section.
    // Ensures that curl_global_cleanup()
    // is called once by
    // all current client instances.
    {
        std::lock_guard<std::mutex> lock_guard(shared_lock);

        client_is_live = false;
        client_thread.join();
        --live_clients;

        // True when this was the last live 'Client'.
        if (live_clients == 0)
            curl_global_cleanup();
    }
    curl_easy_cleanup(base_request);
    curl_multi_cleanup(multi_request);
    curl_share_cleanup(shared_cache);
}

void CppEth::Client::request_consumer() {
    while(client_is_live)
    {
        // Send requests.
        int dummy = 0;

        CURLMcode curl_multi_code = curl_multi_perform(multi_request, &dummy);

        if(curl_multi_code != CURLM_OK)
            throw std::runtime_error(curl_multi_strerror(curl_multi_code));

        // Clean up promises and requests
        // that have been fulfilled.
        CURLMsg* message = curl_multi_info_read(multi_request, &dummy);

        while(message != nullptr)
        {
            std::cout << "here" << '\n';

            if(message->data.result != CURLE_OK)
                throw std::runtime_error(curl_easy_strerror(message->data.result));

            CURL* request = message->easy_handle;

            printRequestInfo(request);

            map_request_to_promise.erase(request);

            message = curl_multi_info_read(multi_request, &dummy);

            curl_multi_remove_handle(multi_request, request);
            curl_easy_cleanup(request);
        }
    }
}

std::future<std::string> CppEth::Client::async_call(
    const std::string &to,
    const std::string &data
){
    CURL* request = curl_easy_duphandle(base_request);

    std::string fields =
            R"({"jsonrpc":"2.0", "method":"eth_call", "params":[{"to":")"
            + to
            + R"(", "data":")"
            + data
            + R"("}], "id":0})";
    curl_easy_setopt(request, CURLOPT_COPYPOSTFIELDS, fields.c_str());

    // 'multi_request' should now share
    // TLS/ SSL info between individual request.
    curl_easy_setopt(base_request, CURLOPT_SHARE, shared_cache);

    // THIS IS DANGEROUS!
    //
    // Unless over LAN. Set up your own node,
    // because it is WAY faster.
    curl_easy_setopt(request, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(request, CURLOPT_SSL_VERIFYPEER, 0);

    // Store promise in map and
    // retreive associated future
    // before we send off the request!
    std::future<std::string> future = map_request_to_promise[request].get_future();

    curl_easy_setopt(request, CURLOPT_WRITEDATA, &map_request_to_promise[request]);

    curl_multi_add_handle(multi_request, request);

    return future;
}