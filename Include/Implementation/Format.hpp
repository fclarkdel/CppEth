#ifndef SOLIDITYCONTRACTABITRANSLATOR_FORMAT_HPP
#define SOLIDITYCONTRACTABITRANSLATOR_FORMAT_HPP

#include <string>

namespace SolidityContractInterface::Format {
    namespace Implementation {
        bool encodingHasPrefix(std::string_view encoding);
    }
    std::string_view trimPrefixIfPresent(std::string_view encoding);

    std::string padLeft(const char PADDING_CHAR, const size_t PADDING_AMOUNT, std::string stringToPad);

    std::string padRight(const char PADDING_CHAR, const size_t PADDING_AMOUNT, std::string stringToPad);
}
#endif