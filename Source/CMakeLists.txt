set(
    INCLUDE_DIRECTORY
    ../Include/
)
set(
    EXTERNAL_DIRECTORY
    ../External/
)
add_library(
    Source

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
target_link_libraries(
    Source

    PUBLIC

    boost_multiprecision
    cryptopp
)
target_include_directories(
    Source

    PUBLIC

    ${INCLUDE_DIRECTORY}

    ${EXTERNAL_DIRECTORY}multiprecision-Boost_1_82_0/include
)