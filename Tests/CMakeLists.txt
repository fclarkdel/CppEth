set(
    INCLUDE_DIRECTORY
    ../Include/
)
set(
    EXTERNAL_DIRECTORY
    ../External/
)
add_executable(
    Tests

    AbiTest.cpp
    FormatTest.cpp
    HexTest.cpp
    KeccakTest.cpp
)
target_link_libraries(
    Tests

    PRIVATE

    Source
    gmock_main
)
target_include_directories(
    Tests

    PRIVATE

    ${EXTERNAL_DIRECTORY}googletest/googletest/include
)
enable_testing()