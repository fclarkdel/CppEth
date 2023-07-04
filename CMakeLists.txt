cmake_minimum_required(VERSION 3.25)

project(CppEth)

set(
    CMAKE_CXX_STANDARD
    23
)
add_subdirectory(Source)

# Library implementors target this.
add_library(
    CppEth
    INTERFACE
)
target_link_libraries(
    CppEth
    INTERFACE
    CppEth_Source
)
target_include_directories(
    CppEth
    INTERFACE
    Source
)