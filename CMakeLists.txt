cmake_minimum_required(VERSION 3.25)

project(SolidityContractInterface)

set(CMAKE_CXX_STANDARD 23)

# Project variables.
set(
    SOURCE_NAME
    ${PROJECT_NAME}_Source
)
set(
    TESTS_NAME
    ${PROJECT_NAME}_Tests
)
set(
    EXTERNAL_DIRECTORY
    ${PROJECT_SOURCE_DIR}/External/
)
set(
    INCLUDE_DIRECTORY
    ${PROJECT_SOURCE_DIR}/Include/
)
set(
    TESTS_DIRECTORY
    ${PROJECT_SOURCE_DIR}/Tests/
)
add_subdirectory(Source)

# Tests should only be included
# when this is the top level project.
if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(
        ${TESTS_DIRECTORY}
        ${TESTS_DIRECTORY}Build
    )
    enable_testing()
endif()