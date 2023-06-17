cmake_minimum_required(VERSION 3.25)

project(SolidityContractInterface)

set(
    CMAKE_CXX_STANDARD
    23
)
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
    EXTERNAL_BUILD_DIRECTORY
    ${PROJECT_BINARY_DIR}/External/
)
set(
    INCLUDE_DIRECTORY
    ${PROJECT_SOURCE_DIR}/Include/
)
add_subdirectory(Source)

# Tests should only be included
# when this is the top level project.
if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(Tests)
    enable_testing()
endif()

# Library implementors target this.
add_library(
    ${PROJECT_NAME}
    INTERFACE
)
target_link_libraries(
    ${PROJECT_NAME}
    INTERFACE
    ${SOURCE_NAME}
)
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    ${INCLUDE_DIRECTORY}
)