#ifndef SOLIDITYCONTRACTABITRANSLATOR_HEX_HPP
#define SOLIDITYCONTRACTABITRANSLATOR_HEX_HPP

#include <string>

#include "Format.hpp"
#include "Types.hpp"

namespace SolidityContractInterface::Hex {
    namespace Implementation {
        unsigned int hexCharToDecUnsignedInt(char hexDigit);

        char decUnsignedIntToHexChar(unsigned int digitValue);

        template<typename Element>
        Element decodeElement(std::string_view encoding) {
            throw std::invalid_argument(
                    "Non-specialized template parameter specified. Template parameter must be specialized to use this function."
            );
        }

        template<>
        Types::uint256 decodeElement(std::string_view encoding);

        template<>
        Types::int256 decodeElement(std::string_view encoding);

        template<>
        Types::string decodeElement(std::string_view encoding);

        template<typename Element>
        std::string encodeElement(Element element) {
            throw std::invalid_argument(
                    "Non-specialized template parameter specified. Template parameter must be specialized to use this function."
            );
        }

        template<>
        std::string encodeElement(Types::uint256 element);

        template<>
        std::string encodeElement(Types::int256 element);

        template<>
        std::string encodeElement(Types::string element);
    }

    template<typename Element>
    Element decode(std::string_view encoding) {
        if (encoding.size() >= 2)
            encoding = Format::trimPrefixIfPresent(encoding);

        if (encoding.empty())
            throw std::invalid_argument("Encoding must be non-empty.");

        return Implementation::decodeElement<Element>(encoding);
    }

    template<typename Element>
    std::string encode(Element element) {
        return Implementation::encodeElement<Element>(element);
    }
}
#endif