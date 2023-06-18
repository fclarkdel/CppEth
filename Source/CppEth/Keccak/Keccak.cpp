#include <CppEth/Keccak/Keccak.hpp>

#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/keccak.h>

namespace CppEth::Keccak {
    std::string keccak256(std::string toHash) {
        using namespace CryptoPP;

        Keccak_256 hasher;

        std::string hash;

        StringSource(
                toHash,
                true,
                new HashFilter(
                        hasher,
                        new HexEncoder(
                                new StringSink(
                                        hash
                                )
                        ),
                        false
                )
        );
        return hash;
    }
}