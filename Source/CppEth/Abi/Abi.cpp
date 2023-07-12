#include <CppEth/Abi/Abi.hpp>

#include <CppEth/Hex/Hex.hpp>
#include <CppEth/Keccak/Keccak.hpp>

namespace CppEth::Abi::Implementation {
    template<>
    std::tuple<uint256> decodeElement(size_t index, std::string_view encoding) {
        return std::tuple{Hex::decode<uint256>(encoding.substr(index * HEX_DIGITS_IN_WORD, HEX_DIGITS_IN_WORD))};
    }

    template<>
    std::tuple<int256> decodeElement(size_t index, std::string_view encoding) {
        return std::tuple{Hex::decode<int256>(encoding.substr(index * HEX_DIGITS_IN_WORD, HEX_DIGITS_IN_WORD))};
    }

    template<>
    std::tuple<string> decodeElement(size_t index, std::string_view encoding) {
        // Offset is in amount of bytes,
        // so we need to multiply it by 2.
        // Note: 1 byte is 2 hex digits.
        size_t offset = static_cast<size_t>(
                2 * Hex::decode<uint256>(encoding.substr(index * HEX_DIGITS_IN_WORD, HEX_DIGITS_IN_WORD))
        );
        size_t size = 2 * static_cast<size_t>(Hex::decode<uint256>(encoding.substr(offset, HEX_DIGITS_IN_WORD)));

        return std::tuple{Hex::decode<string>(encoding.substr(offset + HEX_DIGITS_IN_WORD, size))};
    }

    std::string functionSelector(std::string functionSignature) {
        const size_t NUM_BYTES = 4;
        const size_t NUM_HEX_DIGITS = NUM_BYTES * 2;

        std::string hash = Keccak::keccak256(functionSignature).substr(0, NUM_HEX_DIGITS);

        for (char &index: hash)
            index = static_cast<char>(std::tolower(index));

        return hash;
    }

    template<>
    std::string encodeArgumentHead(uint256 argument) {
        std::string encoding = Hex::encode(argument);

        return Format::padLeft('0', HEX_DIGITS_IN_WORD - encoding.size(), encoding);
    }

    template<>
    std::string encodeArgumentHead(int256 argument) {
        std::string encoding = Hex::encode(argument);

        // Must pad negative argument encoding
        // with 'f' to compensate
        // for two's complement encoding.
        return Format::padLeft(argument < 0 ? 'f' : '0', HEX_DIGITS_IN_WORD - encoding.size(), encoding);
    }

    template<>
    std::string encodeArgumentHead(string argument) {
        return Format::padLeft('0', HEX_DIGITS_IN_WORD, "");
    }

    template<>
    std::string encodeArgumentTail(std::string encoding, size_t index, uint256 argument) {
        return encoding;
    }

    template<>
    std::string encodeArgumentTail(std::string encoding, size_t index, int256 argument) {
        return encoding;
    }

    template<>
    std::string encodeArgumentTail(std::string encoding, size_t index, string argument) {
        // Offset is the number of bytes
        // between string encoding and beginning
        // of the encoding.
        // Not the number of hex digits.
        // (That's why we divide by 2)
        std::string offset = Hex::encode(static_cast<uint256>(encoding.size() / 2));
        encoding.replace(
                index * HEX_DIGITS_IN_WORD,
                HEX_DIGITS_IN_WORD,
                Format::padLeft('0', HEX_DIGITS_IN_WORD - offset.size(), offset)
        );
        std::string size = Hex::encode(static_cast<uint256>(argument.size()));

        encoding += Format::padLeft('0', HEX_DIGITS_IN_WORD - size.size(), size);

        std::string argumentEncoding = Hex::encode(argument);

        // Dynamic argument encoding
        // must have an amount of bytes
        // that is a multiple of 32.
        size_t sizeToPadTo = argumentEncoding.size();

        if(sizeToPadTo % HEX_DIGITS_IN_WORD)
            sizeToPadTo = ((sizeToPadTo / HEX_DIGITS_IN_WORD) + 1) * HEX_DIGITS_IN_WORD;

        encoding += Format::padRight('0', sizeToPadTo - argumentEncoding.size(), argumentEncoding);

        return encoding;
    }
}