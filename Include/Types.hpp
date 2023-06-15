#ifndef SOLIDITYCONTRACTABITRANSLATOR_TYPES_HPP
#define SOLIDITYCONTRACTABITRANSLATOR_TYPES_HPP

#include <string>

#include "boost/multiprecision/cpp_int.hpp"

namespace SolidityContractInterface::Types{
    using uint256 = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::unsigned_magnitude, boost::multiprecision::checked, void>>;

    using int256 = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::signed_magnitude, boost::multiprecision::checked, void>>;

    using string = std::string;
}
#endif