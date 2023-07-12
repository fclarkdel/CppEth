#ifndef SOLIDITYCONTRACTABITRANSLATOR_ABI_H
#define SOLIDITYCONTRACTABITRANSLATOR_ABI_H

#include <string>
#include <tuple>
#include <stdexcept>

#include <CppEth/Format/Format.hpp>
#include <CppEth/Types.hpp>

namespace CppEth::Abi {
    namespace Implementation {
        const size_t BYTES_IN_WORD = 32;
        const size_t HEX_DIGITS_IN_WORD = BYTES_IN_WORD * 2;

        template<typename Element>
        std::tuple<Element> decodeElement(size_t index, std::string_view encoding) {
            throw std::invalid_argument(
                    "Non-specialized template parameter specified. Template parameter must be specialized to use this function."
            );
        }

        template<>
        std::tuple<uint256> decodeElement(size_t index, std::string_view encoding);

        template<>
        std::tuple<int256> decodeElement(size_t index, std::string_view encoding);

        template<>
        std::tuple<string> decodeElement(size_t index, std::string_view encoding);

        template<typename Element, typename ...Elements>
        std::tuple<Element, Elements...> decodeElements(size_t index, std::string_view encoding) {
            std::tuple<Element> tupleHead = decodeElement<Element>(index, encoding);

            std::tuple<Elements...> tupleTail;

            if constexpr (sizeof...(Elements))
                tupleTail = decodeElements<Elements...>(index + 1, encoding);

            return std::tuple_cat(tupleHead, tupleTail);
        }

        std::string functionSelector(std::string functionSignature);

        template<typename Element>
        std::string encodeArgumentHead(Element argument) {
            throw std::invalid_argument(
                    "Non-specialized template parameter specified. Template parameter must be specialized to use this function."
            );
        }

        template<>
        std::string encodeArgumentHead(uint256 argument);

        template<>
        std::string encodeArgumentHead(int256 argument);

        template<>
        std::string encodeArgumentHead(string argument);

        template<typename Element, typename ...Elements>
        std::string argumentEncodingHead(Element argument, Elements... arguments) {
            std::string encoding = encodeArgumentHead(argument);

            if constexpr(sizeof...(Elements))
                encoding += argumentEncodingHead(arguments...);

            return encoding;
        }

        template<typename Element>
        std::string encodeArgumentTail(std::string encoding, size_t index, Element argument){
            throw std::invalid_argument(
                    "Non-specialized template parameter specified. Template parameter must be specialized to use this function."
            );
        }

        template<>
        std::string encodeArgumentTail(std::string encoding, size_t index, uint256 argument);

        template<>
        std::string encodeArgumentTail(std::string encoding, size_t index, int256 argument);

        template<>
        std::string encodeArgumentTail(std::string encoding, size_t index, string argument);

        template<typename Element, typename ...Elements>
        std::string argumentEncodingTail(std::string encoding, size_t index, Element argument, Elements... arguments){
            encoding = encodeArgumentTail(encoding, index, argument);

            if constexpr(sizeof...(Elements))
                encoding = argumentEncodingTail(encoding, index + 1, arguments...);

            return encoding;
        }

        template<typename Element, typename ...Elements>
        std::string argumentEncoding(Element argument, Elements... arguments) {
            std::string encoding = argumentEncodingHead(argument, arguments...);

            encoding = argumentEncodingTail(encoding, 0, argument, arguments...);

            return encoding;
        }
    }

    template<typename Element, typename ...Elements>
    std::tuple<Element, Elements...> decode(std::string_view encoding) {
        return Implementation::decodeElements<Element, Elements...>(0, Format::trimPrefixIfPresent(encoding));
    }

    template<typename ...Elements>
    std::string encode(std::string functionSignature, Elements... arguments) {
        std::string encoding = "0x" + Implementation::functionSelector(functionSignature);

        if constexpr (sizeof...(Elements))
            encoding += Implementation::argumentEncoding(arguments...);

        return encoding;
    }
}
#endif