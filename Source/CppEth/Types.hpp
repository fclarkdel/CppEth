#ifndef SOLIDITYCONTRACTABITRANSLATOR_TYPES_HPP
#define SOLIDITYCONTRACTABITRANSLATOR_TYPES_HPP

#include <string>
#include <variant>

#include <boost/multiprecision/cpp_int.hpp>

namespace CppEth
{
using uint256 = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::unsigned_magnitude, boost::multiprecision::checked, void>>;

using int256 = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::signed_magnitude, boost::multiprecision::checked, void>>;

using string = std::string;

struct address
{
	explicit address(const char* address)
	{
		if(std::strlen(address) != DATA_LENGTH)
			throw std::invalid_argument("Invalid address length, must be 40 characters long.");

		data = address;
	}
	explicit address(std::string address)
	{
		if(address.length() != DATA_LENGTH)
			throw std::invalid_argument("Invalid address length, must be 40 characters long.");

		data = std::move(address);
	}
	std::string_view
	value()
	{
		return data;
	}

private:
	const std::size_t DATA_LENGTH = 40;
	std::string data;
};

}
#endif