#ifndef SOLIDITYCONTRACTINTERFACE_KECCAK256_H
#define SOLIDITYCONTRACTINTERFACE_KECCAK256_H

#include <string>

namespace CppEth::Keccak {
    std::string keccak256(std::string toHash);
}
#endif