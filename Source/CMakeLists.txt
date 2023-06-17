add_library(
    ${SOURCE_NAME}

    ${INCLUDE_DIRECTORY}Abi.hpp
    ${INCLUDE_DIRECTORY}Format.hpp
    ${INCLUDE_DIRECTORY}Hex.hpp
    ${INCLUDE_DIRECTORY}Keccak.hpp
    ${INCLUDE_DIRECTORY}Types.hpp

    Abi.cpp
    Format.cpp
    Hex.cpp
    Keccak.cpp
)
add_subdirectory(
    ${EXTERNAL_DIRECTORY}cryptopp-cmake
    ${EXTERNAL_DIRECTORY}cryptopp-cmake/Build
)
add_subdirectory(
    ${EXTERNAL_DIRECTORY}multiprecision-Boost_1_82_0
    ${EXTERNAL_DIRECTORY}multiprecision-Boost_1_82_0/Build
)
target_link_libraries(
    ${SOURCE_NAME}

    PUBLIC

    Boost::multiprecision
    cryptopp
)
target_include_directories(
    ${SOURCE_NAME}

    PUBLIC

    ${INCLUDE_DIRECTORY}
)