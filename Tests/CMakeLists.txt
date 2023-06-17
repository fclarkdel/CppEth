add_executable(
    ${TESTS_NAME}

    AbiTest.cpp
    FormatTest.cpp
    HexTest.cpp
    KeccakTest.cpp
)
add_subdirectory(
    ${EXTERNAL_DIRECTORY}googletest
    ${EXTERNAL_DIRECTORY}googletest/Build
)
target_link_libraries(
    ${TESTS_NAME}

    PRIVATE

    ${SOURCE_NAME}
    gmock_main
)