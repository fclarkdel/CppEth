#include <gtest/gtest.h>

#include <CppEth/Abi/Abi.hpp>
#include <CppEth/Client/Client.hpp>

#include <chrono>

TEST(Client, asyncRequestsShouldBeFaster){
    CppEth::Client client("https://eth.llamarpc.com");

    const size_t NUM_REQUESTS = 200;

    // Async requests.
    struct timespec start{};
    struct timespec finish{};

    clock_gettime(CLOCK_MONOTONIC, &start);

    std::vector<std::future<std::string>> futures;
    for(size_t amount = 0; amount < NUM_REQUESTS; ++amount)
        futures.push_back(client.async_call("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48", "0x06fdde03"));

    for(auto& future : futures)
        future.get();

    clock_gettime(CLOCK_MONOTONIC, &finish);
    double asyncTime = (finish.tv_sec - start.tv_sec) + ((finish.tv_nsec - start.tv_nsec) / 1e9);

    // Sync requests.
    clock_gettime(CLOCK_MONOTONIC, &start);

    std::vector<std::string> responses;
    for(size_t amount = 0; amount < NUM_REQUESTS; ++amount)
        responses.push_back(client.async_call("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48", "0x06fdde03").get());

    clock_gettime(CLOCK_MONOTONIC, &finish);
    double syncTime = (finish.tv_sec - start.tv_sec) + ((finish.tv_nsec - start.tv_nsec) / 1e9);

    std::cout << "async time: " << asyncTime << "s\n";
    std::cout << "sync time:  " << syncTime << "s\n";

    EXPECT_GT(syncTime, asyncTime);
}