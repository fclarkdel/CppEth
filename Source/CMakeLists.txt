add_library(
    CppEth_Source

    CppEth/Abi/Abi.cpp
    CppEth/Format/Format.cpp
    CppEth/Hex/Hex.cpp
    CppEth/Keccak/Keccak.cpp
    CppEth/Node/Node.cpp
)
include(FetchContent)

FetchContent_Declare(
    boost-multiprecision
    URL https://github.com/boostorg/multiprecision/archive/refs/tags/Boost_1_82_0.zip
)
FetchContent_Declare(
    cryptopp
    URL https://github.com/abdes/cryptopp-cmake/archive/refs/tags/CRYPTOPP_8_7_0_1.zip
)
FetchContent_Declare(
    daw-json-link
    URL https://github.com/beached/daw_json_link/archive/refs/tags/v3.17.2.zip
)
FetchContent_Declare(
    LibJsonRpc
    GIT_REPOSITORY https://github.com/fclarkdel/LibJsonRpc.git
    GIT_TAG main
)
FetchContent_MakeAvailable(boost-multiprecision cryptopp daw-json-link LibJsonRpc)

target_link_libraries(
    CppEth_Source

    PUBLIC
    Boost::multiprecision
    cryptopp
    daw::daw-json-link
    LibJsonRpc
)
target_include_directories(
    CppEth_Source

    PUBLIC
    ../Source
)
# Tests should only be included
# when this is the top level project.
if(PROJECT_IS_TOP_LEVEL)
    add_executable(
        CppEth_Tests

        CppEth/Abi/Abi.test.cpp
        CppEth/Format/Format.test.cpp
        CppEth/Hex/Hex.test.cpp
        CppEth/Keccak/Keccak.test.cpp
        CppEth/Node/Node.test.cpp
    )
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
    )
    FetchContent_MakeAvailable(googletest)

    target_link_libraries(
        CppEth_Tests

        PRIVATE
        CppEth_Source
        gmock_main
    )
    enable_testing()
endif()