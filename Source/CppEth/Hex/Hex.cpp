#include <CppEth/Hex/Hex.hpp>

namespace CppEth::Hex::Implementation{
    unsigned int hexCharToDecUnsignedInt(char hexDigit) {
        unsigned int digitValue = 0;

        switch (hexDigit) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                digitValue = hexDigit - '0';
                break;
            case 'a':
            case 'A':
                digitValue = 10;
                break;
            case 'b':
            case 'B':
                digitValue = 11;
                break;
            case 'c':
            case 'C':
                digitValue = 12;
                break;
            case 'd':
            case 'D':
                digitValue = 13;
                break;
            case 'e':
            case 'E':
                digitValue = 14;
                break;
            case 'f':
            case 'F':
                digitValue = 15;
                break;
            default:
                throw std::invalid_argument("Invalid hex digit.");
        }
        return digitValue;
    }

    char decUnsignedIntToHexChar(unsigned int digitValue) {
        char hexDigit = '\0';

        switch (digitValue) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                hexDigit = static_cast<char>(digitValue + '0');
                break;
            case 10:
                hexDigit = 'a';
                break;
            case 11:
                hexDigit = 'b';
                break;
            case 12:
                hexDigit = 'c';
                break;
            case 13:
                hexDigit = 'd';
                break;
            case 14:
                hexDigit = 'e';
                break;
            case 15:
                hexDigit = 'f';
                break;
            default:
                throw std::invalid_argument("Invalid digit value");
        }
        return hexDigit;
    }

    template<>
    Types::uint256 decodeElement(std::string_view encoding) {
        // We are assuming big-endianness.
        Types::uint256 digitValue = 0;
        Types::uint256 accumulateValue = 0;

        // Work from the MSD to the LSD.
        for (size_t index = 0; index < encoding.size(); ++index) {
            digitValue = hexCharToDecUnsignedInt(encoding[index]);

            // A left bit shift of 4 "<< 4"
            // is the same as multiplying by 16.
            digitValue <<= 4 * ((encoding.size() - 1) - index);

            accumulateValue += digitValue;
        }
        return accumulateValue;
    }

    template<>
    Types::int256 decodeElement(std::string_view encoding) {
        // We are assuming big-endianness,
        // and two's complement form.

        // Calculate the value
        // of the encoding
        // not including the MSD.
        Types::int256 accumulateValue = decodeElement<Types::uint256>(encoding.substr(1, encoding.size() - 1));

        // Grab the MSD.
        Types::uint256 digitValue = hexCharToDecUnsignedInt(encoding.front());

        // Go through the bits of the MSD,
        // from LSB to MSB in MSD.
        // Subtract the MSB, add the others.
        const size_t BIN_BASE = 2;
        const size_t NUM_BITS_IN_HEX_DIGIT = 4;
        const size_t NUM_BITS_IN_HEX_ENCODING = 4 * encoding.size();

        for (size_t index = 0; index < NUM_BITS_IN_HEX_DIGIT; ++index) {
            // Grab the value of the bit
            // from the digit.
            Types::uint256 bitValue = digitValue % BIN_BASE;
            digitValue /= BIN_BASE;

            // Raise bit to power
            // of its bit position - 1.
            bitValue <<= (NUM_BITS_IN_HEX_ENCODING - NUM_BITS_IN_HEX_DIGIT) + index;

            // Subtract if MSB,
            // otherwise add.
            if (index == NUM_BITS_IN_HEX_DIGIT - 1)
                accumulateValue -= bitValue;
            else
                accumulateValue += bitValue;
        }
        return accumulateValue;
    }

    template<>
    Types::string decodeElement(std::string_view encoding) {
        // Check for valid size.
        if (encoding.empty() || encoding.size() % 2)
            throw std::invalid_argument("Encoding size must be a non-zero positive multiple of two.");

        // Only works with
        // ASCII encoding for now.
        Types::string ascii;

        for (size_t index = 0; index < encoding.size(); index += 2)
            ascii += static_cast<char>(decodeElement<Types::uint256>(encoding.substr(index, 2)));

        return ascii;
    }

    template<>
    std::string encodeElement(Types::uint256 element) {
        const size_t BASE_HEX = 16;

        std::string encoding;

        while (element > 0) {
            encoding += decUnsignedIntToHexChar(static_cast<unsigned int>(element % BASE_HEX));

            element /= BASE_HEX;
        }
        std::reverse(encoding.begin(), encoding.end());

        return encoding;
    }

    template<>
    std::string encodeElement(Types::int256 element) {
        // Encoding using two's complement.
        if (element > 0)
            return encodeElement(static_cast<Types::uint256>(element));

        const size_t BASE_HEX = 16;

        // Get the magnitude
        // of the element.
        element *= -1;

        // Calculate the number of
        // digits required to represent
        // the magnitude of the element
        // in hexadecimal.
        auto temp = element;
        size_t numHexDigits = 0;

        while (temp > 0) {
            temp /= BASE_HEX;
            ++numHexDigits;
        }
        // Calculate value to subtract
        // magnitude of element from.
        //
        // i.e. 16 ^ (numHexDigits).
        //
        // When we subtract the magnitude
        // of the element from this value
        // we will get the two's complement -
        // it just needs to be encoded to hex.
        temp = 1 << 4 * (numHexDigits);

        return encodeElement(static_cast<Types::uint256>(temp - element));
    }

    template<>
    std::string encodeElement(Types::string element) {
        std::string encoding;

        for (char c: element)
            encoding += encodeElement<Types::uint256>(c);

        return encoding;
    }
}