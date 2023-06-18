#include <CppEth/Format/Format.hpp>

namespace CppEth::Format {
    namespace Implementation {
        bool encodingHasPrefix(std::string_view encoding) {
            return encoding.substr(0, 2) == "0x" || encoding.substr(0, 2) == "0X";
        }
    }

    std::string_view trimPrefixIfPresent(std::string_view encoding) {
        if (Implementation::encodingHasPrefix(encoding))
            return encoding.substr(2, encoding.size() - 2);

        return encoding;
    }

    std::string padLeft(const char PADDING_CHAR, const size_t PADDING_AMOUNT, std::string stringToPad) {
        std::string padding(PADDING_AMOUNT, PADDING_CHAR);

        return padding + stringToPad;
    }

    std::string padRight(const char PADDING_CHAR, const size_t PADDING_AMOUNT, std::string stringToPad) {
        std::string padding(PADDING_AMOUNT, PADDING_CHAR);

        return stringToPad + padding;
    }
}