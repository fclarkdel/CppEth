add_library(
    ${SOURCE_NAME}
    CppEth/Abi/Abi.cpp
    CppEth/Format/Format.cpp
    CppEth/Hex/Hex.cpp
    CppEth/Keccak/Keccak.cpp
)
if(NOT TARGET Boost::multiprecision)
    add_subdirectory(
        ${EXTERNAL_DIRECTORY}multiprecision-Boost_1_82_0
        ${EXTERNAL_BUILD_DIRECTORY}multiprecision-Boost_1_82_0
    )
endif()

if(NOT TARGET cryptopp)
    add_subdirectory(
        ${EXTERNAL_DIRECTORY}cryptopp-cmake
        ${EXTERNAL_BUILD_DIRECTORY}cryptopp-cmake
    )
endif()

target_link_libraries(
    ${SOURCE_NAME}
    PUBLIC
    Boost::multiprecision
    cryptopp
)
target_include_directories(
    ${SOURCE_NAME}
    PUBLIC
    ../Source
)
# Tests should only be included
# when this is the top level project.
if(PROJECT_IS_TOP_LEVEL)
    add_executable(
        ${TESTS_NAME}
        CppEth/Abi/Abi.test.cpp
        CppEth/Format/Format.test.cpp
        CppEth/Hex/Hex.test.cpp
        CppEth/Keccak/Keccak.test.cpp
    )
    if(NOT TARGET gmock_main)
        add_subdirectory(
            ${EXTERNAL_DIRECTORY}googletest
            ${EXTERNAL_BUILD_DIRECTORY}googletest
        )
    endif()
    target_link_libraries(
        ${TESTS_NAME}
        PRIVATE
        ${SOURCE_NAME}
        gmock_main
    )
    enable_testing()
endif()