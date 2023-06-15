cmake_minimum_required(VERSION 3.25)

project(SolidityContractInterface)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(External/cryptopp-cmake)
add_subdirectory(External/googletest)
add_subdirectory(External/multiprecision-Boost_1_82_0)
add_subdirectory(Source)
add_subdirectory(Tests)