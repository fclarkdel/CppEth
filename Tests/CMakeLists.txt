add_executable(
    ${TESTS_NAME}
    AbiTest.cpp
    FormatTest.cpp
    HexTest.cpp
    KeccakTest.cpp
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